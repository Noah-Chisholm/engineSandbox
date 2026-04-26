#include "renderable.h"
#include "renderHandler.h"
#include "logging.h"

renderable::renderable(name _str, std::shared_ptr<meshDataGPU> _mesh, transform _transform)
	: baseObject(_str),
	sceneInterface(_transform),
	tickableInterface(),
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
	renderHandler::getInstance().createRenderableConstantBuffer(*this);

	setVisible(true);
	setActive(true);
}

DirectX::XMFLOAT4X4 renderable::makeModelMatrix(const transform& trans) {
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
			trans.rotation.x,
			trans.rotation.y,
			trans.rotation.z
		);

	DirectX::XMMATRIX model =
		scale * rotation * translation;

	DirectX::XMFLOAT4X4 result;
	DirectX::XMStoreFloat4x4(&result, model);

	return result;
}

void renderable::tick(float dt) {
	trans.location.x += vel;
	if (trans.location.x >= 1 || trans.location.x <= -1)
		vel *= -1;
	trans.scale.x += scaleSpeed;
	trans.scale.y += scaleSpeed;
	if (trans.scale.x >= 1.5 || trans.scale.x <= 0.5)
		scaleSpeed *= -1;

	trans.rotation.z += rotSpeed;
}
