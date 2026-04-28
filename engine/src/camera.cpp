#include "camera.h"

camera::camera(core::Name _str, core::math::STransform _transform, cameraData _data)
	: baseObject(_str),
	sceneInterface(_transform),
	data(_data)
{
}

DirectX::XMMATRIX camera::makeViewMatrix() const {
    DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(
        trans.location.x,
        trans.location.y,
        trans.location.z,
        1.0f
    );
    core::math::SVector forwardVector = getForwardVector();
    DirectX::XMVECTOR forwardDirection = DirectX::XMVectorSet(
        forwardVector.x,
        forwardVector.y,
        forwardVector.z,
        0.0f
    );

    core::math::SVector upVector = getUpVector();
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

DirectX::XMMATRIX camera::makeProjectionMatrix() const {
    DirectX::XMMATRIX result = DirectX::XMMatrixPerspectiveFovLH(
        data.fovYRadians,
        data.aspectRatio,
        data.nearPlane,
        data.farPlane
    );
    return result;
}