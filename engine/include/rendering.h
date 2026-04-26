#pragma once

#include "colors.h"
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "name.h"
#include "fVector.h"
#include <DirectXMath.h>

struct Vertex {
	fVector position;
	FcolorRGBA color;
};

struct meshDataCPU {
	name key;
	std::vector<Vertex> verts;
	std::vector<uint32_t> indicies;
};

struct meshDataGPU {
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	uint32_t indexCount = 0;
};

struct transform {
	fVector location;
	fVector rotation;
	fVector scale;
};

struct renderableConstants {
	DirectX::XMFLOAT4X4 projectedModelViewMatrix;
};