#pragma once

#include <stdint.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include "rendering.h"
#include "renderable.h"
#include "camera.h"
#include "name.h"

class renderable;

class renderHandler {
private:
    renderHandler();
    ~renderHandler() = default;

public:
    static renderHandler& getInstance();

    void init(uint32_t _width, uint32_t _height, HWND _renderWindow, std::shared_ptr<camera> _activeCamera);
    void render();
    void cleanUp();

    bool unregisterRender(std::shared_ptr<renderable> registrant);

    bool registerRender(std::shared_ptr<renderable> registrant);
    void createRenderableConstantBuffer(renderable& item);
    void updateRenderableConstantBuffer(renderable& item);

    std::shared_ptr<meshDataGPU> createGpuMesh(const meshDataCPU& meshData);

private:
    std::vector<std::shared_ptr<renderable>> renderables;
    std::unordered_map<name, std::shared_ptr<meshDataGPU>, nameHasher> loadedMeshes;

    static const uint8_t frameCount = 2;

    uint32_t width = 0;
    uint32_t height = 0;
    HWND renderWindow = nullptr;
    std::shared_ptr<camera> activeCamera;

    // Core D3D12 objects.
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

    // Swap-chain back buffers.
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[frameCount];

    // Render target view descriptors for the back buffers.
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize = 0;

    // Depth target view descriptors
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

    // Temporary single-pipeline path.
    // Later this should move toward a material/pipeline abstraction,
    // not into the mesh.
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

    // Synchronization objects.
    uint32_t m_frameIndex = 0;
    HANDLE m_fenceEvent = nullptr;
    Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
    uint64_t m_fenceValue = 0;

private:
    void loadPipeline();
    void loadRendererAssets();
    void populateCommandList();

    void waitForPreviousFrame();

    void ThrowIfFailed(HRESULT hr);
    void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
};