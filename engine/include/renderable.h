#pragma once

#include "rendering/renderingCommon.h"
#include "baseObject.h"
#include "sceneInterface.h"
#include "tickableInterface.h"

class renderable : public baseObject, public sceneInterface {
public:
	renderable(core::Name _Str, std::shared_ptr<rendering::meshDataGPU> _mesh, core::math::STransform _transform);
	void setVisible(bool newVisible);
	
	std::shared_ptr<rendering::meshDataGPU> mesh = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer = nullptr;
	std::uint8_t* mappedConstantBuffer = nullptr;

	virtual void init() override;
	DirectX::XMFLOAT4X4 makeModelMatrix(const core::math::STransform& trans);

private:
protected:
	bool isVisible = false;
};