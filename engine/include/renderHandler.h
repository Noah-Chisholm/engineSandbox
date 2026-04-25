#include <stdint.h>
#include "rendering.h"

class renderHandler {
	private:
        renderHandler();
        ~renderHandler() = default;
    public:
        static renderHandler& getInstance();
		void init(uint32_t _width, uint32_t _height, HWND _renderWindow, uint8_t _frameCount = 2);
		void update();
		void render();
		void cleanUp();
	private:
		static const uint8_t frameCount = 2;
        uint32_t width;
        uint32_t height;
        HWND renderWindow;

        // Pipeline objects.
        D3D12_VIEWPORT m_viewport;
        D3D12_RECT m_scissorRect;
        
        Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D12Device> m_device;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[frameCount];
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
        UINT m_rtvDescriptorSize;

        // App resources.
        Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

        // Synchronization objects.
        uint32_t m_frameIndex;
        HANDLE m_fenceEvent;
        Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
        uint64_t m_fenceValue;

        void loadPipeline();
        void loadAssets();
        void populateCommandList();
        void waitForPreviousFrame();
        void ThrowIfFailed(HRESULT hr);
        void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter);
};