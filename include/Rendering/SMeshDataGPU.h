#pragma once

#include <DirectXMath.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <cstddef>

#include "ThirdParty/Rendering/d3dx12.h"
#include "Assets/SMaterialDataCPU.h"

namespace Rendering {
	struct SMeshDataGPU {
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;

		std::size_t indexCount = 0;
	};

	struct SRenderableConstants {
		DirectX::XMFLOAT4X4 projectedModelViewMatrix;
		Assets::SMaterialConstants materialConstants;
	};
}