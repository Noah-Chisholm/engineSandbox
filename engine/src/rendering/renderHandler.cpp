#include "rendering/renderHandler.h"
#include <stdexcept>
#include <d3dcompiler.h>
#include "renderable.h"

renderHandler::renderHandler() {

}

renderHandler& renderHandler::getInstance() {
    static renderHandler inst;
    return inst;
}

bool renderHandler::registerRender(std::shared_ptr<renderable> registrant) {
    renderables.push_back(registrant);
    return true;
}

bool renderHandler::unregisterRender(std::shared_ptr<renderable> registrant) {
    int counter = 0;
    for (auto obj : renderables)
        if (obj == registrant) {
            renderables.erase(renderables.begin() + counter);
            return true;
        }
        else counter++;
    return false;
}

void renderHandler::init(uint32_t _width, uint32_t _height, HWND _renderWindow, std::shared_ptr<camera> _activeCamera) {
    //m_renderTargets = new Microsoft::WRL::ComPtr<ID3D12Resource>[_frameCount];
    width = _width;
    height = _height;
    //frameCount = _frameCount;
    renderWindow = _renderWindow;
    activeCamera = _activeCamera;

    loadPipeline();
    loadRendererAssets();
}

void renderHandler::render() {
    // Record all the commands we need to render the scene into the command list.
    populateCommandList();

    // Execute the command list.
    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame.
    ThrowIfFailed(m_swapChain->Present(1, 0));

    waitForPreviousFrame();
}

void renderHandler::cleanUp() {
    waitForPreviousFrame();
    CloseHandle(m_fenceEvent);
}

void renderHandler::ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr))
    {
        throw std::runtime_error("DirectX API Error");
    }
}

void renderHandler::loadPipeline() {
    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter);

    ThrowIfFailed(D3D12CreateDevice(
        hardwareAdapter.Get(),
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&m_device)
    ));

    // Describe and create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = frameCount;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = renderWindow;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    ThrowIfFailed(factory->CreateSwapChain(
        m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
        &swapChainDesc,
        &swapChain
    ));

    ThrowIfFailed(swapChain.As(&m_swapChain));

    // This sample does not support fullscreen transitions.
    ThrowIfFailed(factory->MakeWindowAssociation(renderWindow, DXGI_MWA_NO_ALT_ENTER));

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Create descriptor heaps.
    {
        // Describe and create a render target view (RTV) descriptor heap.
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = frameCount;
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

        m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        ThrowIfFailed(m_device->CreateDescriptorHeap(
            &dsvHeapDesc,
            IID_PPV_ARGS(&m_dsvHeap)
        ));
    }

    D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
    depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);

    CD3DX12_RESOURCE_DESC depthDesc =
        CD3DX12_RESOURCE_DESC::Tex2D(
            DXGI_FORMAT_D32_FLOAT,
            width,
            height,
            1,
            0,
            1,
            0,
            D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
        );

    ThrowIfFailed(m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &depthDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &depthOptimizedClearValue,
        IID_PPV_ARGS(&m_depthStencilBuffer)
    ));

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

    m_device->CreateDepthStencilView(
        m_depthStencilBuffer.Get(),
        &dsvDesc,
        m_dsvHeap->GetCPUDescriptorHandleForHeapStart()
    );

    // Create frame resources.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame.
        for (UINT n = 0; n < frameCount; n++)
        {
            ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
            m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, m_rtvDescriptorSize);
        }
    }

    ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

void renderHandler::loadRendererAssets() {
    // Create an empty root signature.
    {
        CD3DX12_ROOT_PARAMETER rootParameters[1];

        rootParameters[0].InitAsConstantBufferView(0);

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

        rootSignatureDesc.Init(
            _countof(rootParameters),
            rootParameters,
            0,
            nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
        );

        Microsoft::WRL::ComPtr<ID3DBlob> signature;
        Microsoft::WRL::ComPtr<ID3DBlob> error;

        ThrowIfFailed(D3D12SerializeRootSignature(
            &rootSignatureDesc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &signature,
            &error
        ));

        ThrowIfFailed(m_device->CreateRootSignature(
            0,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(&m_rootSignature)
        ));
    }

    // Create the pipeline state, which includes compiling and loading shaders.
    {
        Microsoft::WRL::ComPtr<ID3DBlob> vertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> pixelShader;

        UINT compileFlags = 0;

        ThrowIfFailed(D3DCompileFromFile(L"D:\\personal one drive\\OneDrive\\Documents\\engineSandbox\\shaders\\pixel\\pixelShader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
        ThrowIfFailed(D3DCompileFromFile(L"D:\\personal one drive\\OneDrive\\Documents\\engineSandbox\\shaders\\vertex\\vertexShader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));

        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = m_rootSignature.Get();
        psoDesc.VS = { reinterpret_cast<UINT8*>(vertexShader->GetBufferPointer()), vertexShader->GetBufferSize() };
        psoDesc.PS = { reinterpret_cast<UINT8*>(pixelShader->GetBufferPointer()), pixelShader->GetBufferSize() };
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = TRUE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

        ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
    }

    // Create the command list.
    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));

    // Command lists are created in the recording state, but there is nothing
    // to record yet. The main loop expects it to be closed, so close it now.
    ThrowIfFailed(m_commandList->Close());

    // Create synchronization objects and wait until assets have been uploaded to the GPU.
    {
        ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
        m_fenceValue = 1;

        // Create an event handle to use for frame synchronization.
        m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_fenceEvent == nullptr)
        {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
        }

        // Wait for the command list to execute; we are reusing the same command 
        // list in our main loop but for now, we just want to wait for setup to 
        // complete before continuing.
        waitForPreviousFrame();
    }
}

void renderHandler::populateCommandList() {
    if (!activeCamera) {
        throw std::runtime_error("renderHandler has no active camera");
    }

    ThrowIfFailed(m_commandAllocator->Reset());

    ThrowIfFailed(m_commandList->Reset(
        m_commandAllocator.Get(),
        m_pipelineState.Get()
    ));

    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

    m_commandList->RSSetViewports(
        1,
        &(activeCamera->data.viewport)
    );

    m_commandList->RSSetScissorRects(
        1,
        &(activeCamera->data.scissorRect)
    );

    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_renderTargets[m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    m_commandList->ResourceBarrier(1, &barrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        m_frameIndex,
        m_rtvDescriptorSize
    );

    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
        m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

    m_commandList->OMSetRenderTargets(
        1,
        &rtvHandle,
        FALSE,
        &dsvHandle
    );

    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };

    m_commandList->ClearRenderTargetView(
        rtvHandle,
        clearColor,
        0,
        nullptr
    );

    m_commandList->ClearDepthStencilView(
        dsvHandle,
        D3D12_CLEAR_FLAG_DEPTH,
        1.0f,
        0,
        0,
        nullptr
    );

    m_commandList->IASetPrimitiveTopology(
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    for (const std::shared_ptr<renderable>& item : renderables) {
        if (item == nullptr || item->mesh == nullptr) {
            continue;
        }

        updateRenderableConstantBuffer(*item);

        m_commandList->SetGraphicsRootConstantBufferView(
            0,
            item->constantBuffer->GetGPUVirtualAddress()
        );

        m_commandList->IASetVertexBuffers(
            0,
            1,
            &item->mesh->vertexBufferView
        );

        m_commandList->IASetIndexBuffer(
            &item->mesh->indexBufferView
        );

        m_commandList->DrawIndexedInstanced(
            item->mesh->indexCount,
            1,
            0,
            0,
            0
        );
    }

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_renderTargets[m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );

    m_commandList->ResourceBarrier(1, &barrier);

    ThrowIfFailed(m_commandList->Close());
}

void renderHandler::waitForPreviousFrame() {
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
    // This is code implemented as such for simplicity. More advanced samples 
    // illustrate how to use fences for efficient resource usage.

    // Signal and increment the fence value.
    const UINT64 fence = m_fenceValue;
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fence));
    m_fenceValue++;

    // Wait until the previous frame is finished.
    if (m_fence->GetCompletedValue() < fence)
    {
        ThrowIfFailed(m_fence->SetEventOnCompletion(fence, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

void renderHandler::GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;
    for (uint32_t adapterIndex = 0; ; ++adapterIndex)
    {
        IDXGIAdapter1* pAdapter = nullptr;
        if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(adapterIndex, &pAdapter))
        {
            // No more adapters to enumerate.
            break;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the
        // actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
        {
            *ppAdapter = pAdapter;
            return;
        }
        pAdapter->Release();
    }
}

std::shared_ptr<meshDataGPU> renderHandler::createGpuMesh(const meshDataCPU& meshData) {
    auto mapResult = loadedMeshes.find(meshData.key);
    if (mapResult != loadedMeshes.end()) {
        return mapResult->second;
    }

    if (m_device == nullptr) {
        throw std::runtime_error("createGpuMesh called before m_device was initialized.");
    }

    if (meshData.verts.empty()) {
        throw std::runtime_error("createGpuMesh received a mesh with no vertices.");
    }

    if (meshData.indicies.empty()) {
        throw std::runtime_error("createGpuMesh received a mesh with no indices.");
    }

    auto result = std::make_shared<meshDataGPU>();

    // ------------------------------------------------------------
    // Vertex buffer
    // ------------------------------------------------------------
    {
        const UINT vertexBufferSize =
            static_cast<UINT>(meshData.verts.size() * sizeof(Vertex));

        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

        CD3DX12_RESOURCE_DESC bufferDesc =
            CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

        ThrowIfFailed(m_device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&result->vertexBuffer)
        ));

        if (result->vertexBuffer == nullptr) {
            throw std::runtime_error("vertexBuffer was null after CreateCommittedResource.");
        }

        UINT8* vertexDataBegin = nullptr;
        CD3DX12_RANGE readRange(0, 0);

        ThrowIfFailed(result->vertexBuffer->Map(
            0,
            &readRange,
            reinterpret_cast<void**>(&vertexDataBegin)
        ));

        std::memcpy(
            vertexDataBegin,
            meshData.verts.data(),
            vertexBufferSize
        );

        result->vertexBuffer->Unmap(0, nullptr);

        result->vertexBufferView.BufferLocation =
            result->vertexBuffer->GetGPUVirtualAddress();

        result->vertexBufferView.StrideInBytes =
            sizeof(Vertex);

        result->vertexBufferView.SizeInBytes =
            vertexBufferSize;
    }

    // ------------------------------------------------------------
    // Index buffer
    // ------------------------------------------------------------
    {
        const UINT indexBufferSize =
            static_cast<UINT>(meshData.indicies.size() * sizeof(std::uint32_t));

        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

        CD3DX12_RESOURCE_DESC bufferDesc =
            CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize);

        ThrowIfFailed(m_device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &bufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&result->indexBuffer)
        ));

        if (result->indexBuffer == nullptr) {
            throw std::runtime_error("indexBuffer was null after CreateCommittedResource.");
        }

        UINT8* indexDataBegin = nullptr;
        CD3DX12_RANGE readRange(0, 0);

        ThrowIfFailed(result->indexBuffer->Map(
            0,
            &readRange,
            reinterpret_cast<void**>(&indexDataBegin)
        ));

        std::memcpy(
            indexDataBegin,
            meshData.indicies.data(),
            indexBufferSize
        );

        result->indexBuffer->Unmap(0, nullptr);

        result->indexBufferView.BufferLocation =
            result->indexBuffer->GetGPUVirtualAddress();

        result->indexBufferView.SizeInBytes =
            indexBufferSize;

        result->indexBufferView.Format =
            DXGI_FORMAT_R32_UINT;
    }

    result->indexCount =
        static_cast<uint32_t>(meshData.indicies.size());

    loadedMeshes.emplace(meshData.key, result);
    return result;
}

void renderHandler::createRenderableConstantBuffer(renderable& item) {
    if (m_device == nullptr) {
        throw std::runtime_error("Cannot create renderable constant buffer before D3D12 device exists.");
    }

    const UINT constantBufferSize =
        (sizeof(renderableConstants) + 255) & ~255;

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

    CD3DX12_RESOURCE_DESC bufferDesc =
        CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);

    ThrowIfFailed(m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&item.constantBuffer)
    ));

    CD3DX12_RANGE readRange(0, 0);

    ThrowIfFailed(item.constantBuffer->Map(
        0,
        &readRange,
        reinterpret_cast<void**>(&item.mappedConstantBuffer)
    ));

    updateRenderableConstantBuffer(item);
}

void renderHandler::updateRenderableConstantBuffer(renderable& item) {
    if (item.mappedConstantBuffer == nullptr) {
        throw std::runtime_error("Renderable constant buffer is not mapped.");
    }

    renderableConstants constants = {};

    DirectX::XMFLOAT4X4 modelFloat =
        item.makeModelMatrix(item.trans);

    DirectX::XMMATRIX model =
        DirectX::XMLoadFloat4x4(&modelFloat);

    DirectX::XMMATRIX view =
        activeCamera->makeViewMatrix();

    DirectX::XMMATRIX projection =
        activeCamera->makeProjectionMatrix();

    DirectX::XMMATRIX modelViewProjection =
        model * view * projection;

    DirectX::XMStoreFloat4x4(
        &constants.projectedModelViewMatrix,
        modelViewProjection
    );

    std::memcpy(
        item.mappedConstantBuffer,
        &constants,
        sizeof(constants)
    );
}