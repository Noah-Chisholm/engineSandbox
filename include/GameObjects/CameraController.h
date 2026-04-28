#pragma once

#include "GameObjects/Camera.h"
#include "Input/InputHandler.h"
#include "GameObjects/CoreObjects/TickableInterface.h"

namespace GameObjects {
    class CameraController : public Camera, public CoreObjects::TickableInterface {
    public:
        CameraController(Core::Name _str, Core::Math::STransform _transform, SCameraData _data);

        void handleInput(const Input::SInputEvent& event);

        virtual void init() override;
        virtual void tick(float dt) override;

    private:
        bool moveForward = false;
        bool moveBackward = false;
        bool moveLeft = false;
        bool moveRight = false;
        bool moveUp = false;
        bool moveDown = false;

        float yawRadians = 0.0f;
        float pitchRadians = 0.0f;

        float mouseSensitivity = 0.0025f;

        float moveSpeed = 3.0f;
        float accel = 0.1f;
        float maxSpeed = 50.0f;


        Input::InputHandler::InputEventSig inputDel;
    };
}