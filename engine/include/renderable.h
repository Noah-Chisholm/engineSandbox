#pragma once

#include "rendering.h"
#include "baseObject.h"
#include "sceneInterface.h"
#include "tickableInterface.h"
#include "name.h"

class renderable : public baseObject, public sceneInterface {
public:
	renderable(name _Str, std::shared_ptr<meshDataGPU> _mesh, transform _transform);
	void setVisible(bool newVisible);
	
	std::shared_ptr<meshDataGPU> mesh = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer = nullptr;
	UINT8* mappedConstantBuffer = nullptr;

	virtual void init() override;
	DirectX::XMFLOAT4X4 makeModelMatrix(const transform& trans);

private:
protected:
	bool isVisible = false;
};