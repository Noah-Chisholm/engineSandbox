#pragma once

#include "Rendering/RenderingCommon.h"
#include "GameObjects/CoreObjects/BaseObject.h"
#include "GameObjects/CoreObjects/SceneInterface.h"
#include "GameObjects/CoreObjects/TickableInterface.h"

namespace GameObjects {
	class Renderable : public CoreObjects::BaseObject, public CoreObjects::SceneInterface {
	public:
		Renderable(Core::Name _str, std::shared_ptr<Rendering::SMeshDataGPU> _mesh, Core::Math::STransform _transform, std::shared_ptr<Rendering::SMaterialGpuData> _material);
		void setVisible(bool newVisible);

		std::shared_ptr<Rendering::SMeshDataGPU> mesh = nullptr;
		std::shared_ptr<Rendering::SMaterialGpuData> material = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer = nullptr;
		std::uint8_t* mappedConstantBuffer = nullptr;

		virtual void init() override;
		DirectX::XMFLOAT4X4 makeModelMatrix(const Core::Math::STransform& trans);

	private:
	protected:
		bool isVisible = false;
	};
}