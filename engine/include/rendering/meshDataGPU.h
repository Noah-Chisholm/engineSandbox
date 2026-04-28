#pragma once

#include <DirectXMath.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <cstddef>

namespace rendering {
	struct meshDataGPU {
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;

		std::size_t indexCount = 0;
	};

	struct renderableConstants {
		DirectX::XMFLOAT4X4 projectedModelViewMatrix;
	};
}