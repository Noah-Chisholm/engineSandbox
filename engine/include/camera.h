#pragma once

#include "rendering/renderingCommon.h"
#include "baseObject.h"
#include "sceneInterface.h"
#include "core/math/mathCommon.h"

class camera : public baseObject, public sceneInterface {
public:
    struct cameraData {
        float fovYRadians;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        D3D12_VIEWPORT viewport;
        D3D12_RECT scissorRect;

        cameraData(
            float inFovYRadians,
            float inAspectRatio,
            float inNearPlane,
            float inFarPlane,
            std::uint32_t renderWidth,
            std::uint32_t renderHeight
        )
            : fovYRadians(inFovYRadians),
            aspectRatio(inAspectRatio),
            nearPlane(inNearPlane),
            farPlane(inFarPlane),
            viewport{
                0.0f,
                0.0f,
                static_cast<float>(renderWidth),
                static_cast<float>(renderHeight),
                0.0f,
                1.0f
            },
            scissorRect{
                0,
                0,
                static_cast<LONG>(renderWidth),
                static_cast<LONG>(renderHeight)
            }
        {
        }
    };

	camera(core::Name _str, core::math::STransform _transform, cameraData _data);

	cameraData data;

    DirectX::XMMATRIX makeViewMatrix() const;
    DirectX::XMMATRIX makeProjectionMatrix() const;
};