#include "renderable.h"
#include "rendering/renderHandler.h"
#include "logging.h"

renderable::renderable(core::Name _str, std::shared_ptr<meshDataGPU> _mesh, core::math::STransform _transform)
	: baseObject(_str),
	sceneInterface(_transform),
	mesh(_mesh)
{
}

void renderable::setVisible(bool newVisible) {
	if (isVisible != newVisible) {
		isVisible = newVisible;
		if (isVisible)
			renderHandler::getInstance().registerRender(std::static_pointer_cast<renderable>(shared_from_this()));
		else
			renderHandler::getInstance().unregisterRender(std::static_pointer_cast<renderable>(shared_from_this()));
	}
}

void renderable::init() {
	baseObject::init();
	renderHandler::getInstance().createRenderableConstantBuffer(*this);
	setVisible(true);
}

DirectX::XMFLOAT4X4 renderable::makeModelMatrix(const core::math::STransform& trans) {
	DirectX::XMMATRIX scale =
		DirectX::XMMatrixScaling(
			trans.scale.x,
			trans.scale.y,
			trans.scale.z
		);

	DirectX::XMMATRIX translation =
		DirectX::XMMatrixTranslation(
			trans.location.x,
			trans.location.y,
			trans.location.z
		);

	DirectX::XMMATRIX rotation =
		DirectX::XMMatrixRotationRollPitchYaw(
			trans.rotation.pitch,
			trans.rotation.yaw,
			trans.rotation.roll
		);

	DirectX::XMMATRIX model =
		scale * rotation * translation;

	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, model);

	return result;
}