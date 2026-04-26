#include "sceneInterface.h"


sceneInterface::sceneInterface(transform _transform) {
	trans = _transform;
}

fVector sceneInterface::getForwardVector() const {
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(
            trans.rotation.x, // pitch
            trans.rotation.y, // yaw
            trans.rotation.z  // roll
        );

    DirectX::XMVECTOR localForward =
        DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    DirectX::XMVECTOR worldForward =
        DirectX::XMVector3TransformNormal(localForward, rotation);

    DirectX::XMFLOAT3 storedForward;
    DirectX::XMStoreFloat3(&storedForward, worldForward);

    return fVector(
        storedForward.x,
        storedForward.y,
        storedForward.z
    );
}

fVector sceneInterface::getRightVector() const {
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(
            trans.rotation.x, // pitch
            trans.rotation.y, // yaw
            trans.rotation.z  // roll
        );

    DirectX::XMVECTOR localRight =
        DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR worldRight =
        DirectX::XMVector3TransformNormal(localRight, rotation);

    DirectX::XMFLOAT3 storedRight;
    DirectX::XMStoreFloat3(&storedRight, worldRight);

    return fVector(
        storedRight.x,
        storedRight.y,
        storedRight.z
    );
}

fVector sceneInterface::getUpVector() const {
    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationRollPitchYaw(
            trans.rotation.x, // pitch
            trans.rotation.y, // yaw
            trans.rotation.z  // roll
        );

    DirectX::XMVECTOR localUp =
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    DirectX::XMVECTOR worldUp =
        DirectX::XMVector3TransformNormal(localUp, rotation);

    DirectX::XMFLOAT3 storedRight;
    DirectX::XMStoreFloat3(&storedRight, worldUp);

    return fVector(
        storedRight.x,
        storedRight.y,
        storedRight.z
    );
}