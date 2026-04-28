#include "GameObjects/Camera.h"

using namespace GameObjects;

Camera::Camera(Core::Name _str, Core::Math::STransform _transform, SCameraData _data)
	: BaseObject(_str),
	SceneInterface(_transform),
	data(_data)
{
}

DirectX::XMMATRIX Camera::makeViewMatrix() const {
    DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(
        trans.location.x,
        trans.location.y,
        trans.location.z,
        1.0f
    );
    Core::Math::SVector forwardVector = getForwardVector();
    DirectX::XMVECTOR forwardDirection = DirectX::XMVectorSet(
        forwardVector.x,
        forwardVector.y,
        forwardVector.z,
        0.0f
    );

    Core::Math::SVector upVector = getUpVector();
    DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(
        upVector.x,
        upVector.y,
        upVector.z,
        0.0f
    );

    return DirectX::XMMatrixLookToLH(
        eyePosition,
        forwardDirection,
        upDirection
    );
}

DirectX::XMMATRIX Camera::makeProjectionMatrix() const {
    DirectX::XMMATRIX result = DirectX::XMMatrixPerspectiveFovLH(
        data.fovYRadians,
        data.aspectRatio,
        data.nearPlane,
        data.farPlane
    );
    return result;
}