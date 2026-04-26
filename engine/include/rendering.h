#pragma once

#include "colors.h"
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "name.h"
#include <DirectXMath.h>

struct fVector {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	fVector() = default;

	explicit fVector(float i)
		: x(i), y(i), z(i)
	{
	}

	fVector(float inX, float inY, float inZ)
		: x(inX), y(inY), z(inZ)
	{
	}
};

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