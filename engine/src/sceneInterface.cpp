#include "sceneInterface.h"
#include <DirectXMath.h>

sceneInterface::sceneInterface(core::math::STransform _transform) {
	trans = _transform;
}

core::math::SVector sceneInterface::getForwardVector() const {
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

    return core::math::SVector(
        storedForward.x,
        storedForward.y,
        storedForward.z
    );
}

core::math::SVector sceneInterface::getRightVector() const {
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

    return core::math::SVector(
        storedRight.x,
        storedRight.y,
        storedRight.z
    );
}

core::math::SVector sceneInterface::getUpVector() const {
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

    return core::math::SVector(
        storedRight.x,
        storedRight.y,
        storedRight.z
    );
}