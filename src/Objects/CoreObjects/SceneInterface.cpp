#include <DirectXMath.h>

#include "GameObjects/CoreObjects/SceneInterface.h"

using namespace GameObjects::CoreObjects;

SceneInterface::SceneInterface(Core::Math::STransform _transform) {
	trans = _transform;
}

Core::Math::SVector SceneInterface::getForwardVector() const {
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(
            trans.rotation.pitch, // pitch
            trans.rotation.yaw, // yaw
            trans.rotation.roll  // roll
        );

    DirectX::XMVECTOR localForward =
        DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    DirectX::XMVECTOR worldForward =
        DirectX::XMVector3TransformNormal(localForward, rotation);

    DirectX::XMFLOAT3 storedForward;
    DirectX::XMStoreFloat3(&storedForward, worldForward);

    return Core::Math::SVector(
        storedForward.x,
        storedForward.y,
        storedForward.z
    );
}

Core::Math::SVector SceneInterface::getRightVector() const {
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(
            trans.rotation.pitch, // pitch
            trans.rotation.yaw, // yaw
            trans.rotation.roll  // roll
        );

    DirectX::XMVECTOR localRight =
        DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR worldRight =
        DirectX::XMVector3TransformNormal(localRight, rotation);

    DirectX::XMFLOAT3 storedRight;
    DirectX::XMStoreFloat3(&storedRight, worldRight);

    return Core::Math::SVector(
        storedRight.x,
        storedRight.y,
        storedRight.z
    );
}

Core::Math::SVector SceneInterface::getUpVector() const {
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(
            trans.rotation.pitch, // pitch
            trans.rotation.yaw, // yaw
            trans.rotation.roll  // roll
        );

    DirectX::XMVECTOR localUp =
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR worldUp =
        DirectX::XMVector3TransformNormal(localUp, rotation);

    DirectX::XMFLOAT3 storedRight;
    DirectX::XMStoreFloat3(&storedRight, worldUp);

    return Core::Math::SVector(
        storedRight.x,
        storedRight.y,
        storedRight.z
    );
}