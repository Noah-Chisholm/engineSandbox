#include "GameObjects/Renderable.h"
#include "Rendering/RenderHandler.h"
#include "Core/Logging/logging.h"

using namespace GameObjects;

Renderable::Renderable(Core::Name _str, std::shared_ptr<Rendering::SMeshDataGPU> _mesh, Core::Math::STransform _transform)
	: BaseObject(_str),
	SceneInterface(_transform),
	mesh(_mesh)
{
}

void Renderable::setVisible(bool newVisible) {
	if (isVisible != newVisible) {
		isVisible = newVisible;
		if (isVisible)
			Rendering::RenderHandler::getInstance().registerRender(std::static_pointer_cast<Renderable>(shared_from_this()));
		else
			Rendering::RenderHandler::getInstance().unregisterRender(std::static_pointer_cast<Renderable>(shared_from_this()));
	}
}

void Renderable::init() {
	BaseObject::init();
	Rendering::RenderHandler::getInstance().createRenderableConstantBuffer(*this);
	setVisible(true);
}

DirectX::XMFLOAT4X4 Renderable::makeModelMatrix(const Core::Math::STransform& trans) {
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