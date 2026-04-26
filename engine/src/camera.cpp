#include "camera.h"

camera::camera(name _str, transform _transform, cameraData _data)
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

    DirectX::XMVECTOR forwardDirection = DirectX::XMVectorSet(
        0.0f,
        0.0f,
        1.0f,
        0.0f
    );

    DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(
        0.0f,
        1.0f,
        0.0f,
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