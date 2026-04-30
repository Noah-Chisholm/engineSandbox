#pragma once

#include "Rendering/RenderingCommon.h"
#include "GameObjects/Camera.h"
#include "Assets/SMeshDataCPU.h"
#include "GameObjects/Renderable.h"

namespace Rendering {
    class RenderHandler {
    private:
        RenderHandler();
        ~RenderHandler() = default;

    public:
        static RenderHandler& getInstance();

        void init(std::uint32_t _width, std::uint32_t _height, HWND _renderWindow, std::shared_ptr<GameObjects::Camera> _activeCamera);
        void render();
        void cleanUp();

        bool unregisterRender(std::shared_ptr<GameObjects::Renderable> registrant);

        bool registerRender(std::shared_ptr<GameObjects::Renderable> registrant);
        void createRenderableConstantBuffer(GameObjects::Renderable& item);
        void updateRenderableConstantBuffer(GameObjects::Renderable& item);

        std::shared_ptr<SMeshDataGPU> createGpuMesh(const Assets::SMeshDataCPU& meshData);
        std::shared_ptr<SMaterialGpuData> createGpuMaterial(const Assets::SMaterialDataCPU& materialData);

    private:
        std::vector<std::shared_ptr<GameObjects::Renderable>> renderables;
        std::unordered_map<Core::Name, std::shared_ptr<SMeshDataGPU>, Core::Name::NameHasher> loadedMeshes;
        std::unordered_map<Core::Name, std::shared_ptr<SMaterialGpuData>, Core::Name::NameHasher> loadedMaterials;

        static const std::uint8_t frameCount = 2;

        std::uint32_t width = 0;
        std::uint32_t height = 0;
        HWND renderWindow = nullptr;
        std::shared_ptr<GameObjects::Camera> activeCamera;

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
        std::uint32_t m_rtvDescriptorSize = 0;

        // Depth target view descriptors
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

        // Temporary single-pipeline path.
        // Later this should move toward a material/pipeline abstraction,
        // not into the mesh.
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

        // Synchronization objects.
        std::uint32_t m_frameIndex = 0;
        HANDLE m_fenceEvent = nullptr;
        Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
        std::uint64_t m_fenceValue = 0;

    private:
        void loadPipeline();
        void loadRendererAssets();
        void populateCommandList();

        void waitForPreviousFrame();

        void ThrowIfFailed(HRESULT hr);
        void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
    };
}