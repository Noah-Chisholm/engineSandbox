#pragma once

#include "Rendering/RenderingCommon.h"
#include "GameObjects/CoreObjects/BaseObject.h"
#include "GameObjects/CoreObjects/SceneInterface.h"
#include "Core/Math/MathCommon.h"

namespace GameObjects {
    class Camera : public CoreObjects::BaseObject, public CoreObjects::SceneInterface {
    public:
        struct SCameraData {
            float fovYRadians;
            float aspectRatio;
            float nearPlane;
            float farPlane;

            D3D12_VIEWPORT viewport;
            D3D12_RECT scissorRect;

            SCameraData(
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

        Camera(Core::Name _str, Core::Math::STransform _transform, SCameraData _data);

        SCameraData data;

        DirectX::XMMATRIX makeViewMatrix() const;
        DirectX::XMMATRIX makeProjectionMatrix() const;
    };
}