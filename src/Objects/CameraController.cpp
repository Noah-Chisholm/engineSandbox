#include <algorithm>

#include "GameObjects/CameraController.h"
#include "Core/Logging/logging.h"

using namespace GameObjects;

CameraController::CameraController(Core::Name _str, Core::Math::STransform _transform, SCameraData _data) : Camera(_str, _transform, _data) {

}

void CameraController::handleInput(const Input::SInputEvent& event) {
    if (event.type == Input::EInputEventType::KeyDown ||
        event.type == Input::EInputEventType::KeyUp) {

        const bool isDown = event.type == Input::EInputEventType::KeyDown;

        switch (event.key.vk) {
        case 'W': {
            moveForward = isDown;
            break;
        }
        case 'S': {
            moveBackward = isDown;
            break;
        }
        case 'A': {
            moveLeft = isDown;
            break;
        }
        case 'D': {
            moveRight = isDown;
            break;
        }
        case 'E': {
            moveUp = isDown;
            break;
        }
        case 'Q': {
            moveDown = isDown;
            break;
        }
        }
    }
    else if (event.type == Input::EInputEventType::MouseWheel) {
        float speedIncrease = accel;

        const bool shiftHeld = event.mods.hasFlag(Input::EInputFlag::SHIFT);

        const bool controlHeld = event.mods.hasFlag(Input::EInputFlag::CONTROL);

        if (shiftHeld && controlHeld) {
            speedIncrease *= 100.0f;
        }
        else if (shiftHeld || controlHeld) {
            speedIncrease *= 10.0f;
        }

        moveSpeed = std::clamp(
            moveSpeed + speedIncrease * event.wheel.delta,
            0.0f,
            maxSpeed
        );

        Core::Logging::log("movespeed: {}", moveSpeed);
    }
    else if (event.type == Input::EInputEventType::MouseMove) {
        Core::Logging::log("deltaX: {}, deltaY: {}", event.mouseMove.deltaX, event.mouseMove.deltaY);
        yawRadians += event.mouseMove.deltaX * mouseSensitivity;
        pitchRadians += event.mouseMove.deltaY * mouseSensitivity;

        const float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;

        pitchRadians = std::clamp(
            pitchRadians,
            -pitchLimit,
            pitchLimit
        );
        Core::Logging::log("rotation: {}\n", trans.rotation);
        trans.rotation.pitch = pitchRadians;
        trans.rotation.yaw = yawRadians;
        trans.rotation.roll = 0.0f;
    }
}

void CameraController::init() {
    inputDel = Input::InputHandler::InputEventSig::Bind<CameraController, &CameraController::handleInput>(this);
	Input::InputHandler::getInstance().registerForKeyEvent('W', inputDel);
    Input::InputHandler::getInstance().registerForKeyEvent('A', inputDel);
    Input::InputHandler::getInstance().registerForKeyEvent('S', inputDel);
    Input::InputHandler::getInstance().registerForKeyEvent('D', inputDel);
    Input::InputHandler::getInstance().registerForMouseWheel(inputDel);
    Input::InputHandler::getInstance().registerForMouseMove(inputDel);
	setActive(true);
}

void CameraController::tick(float dt) {
    const int forwardState =
        static_cast<int>(moveForward) - static_cast<int>(moveBackward);

    const int rightState =
        static_cast<int>(moveRight) - static_cast<int>(moveLeft);

    const int upState =
        static_cast<int>(moveUp) - static_cast<int>(moveDown);

    const float frameMoveSpeed = moveSpeed * dt;

    trans.location += getForwardVector() *
        (static_cast<float>(forwardState) * frameMoveSpeed);

    trans.location += getRightVector() *
        (static_cast<float>(rightState) * frameMoveSpeed);

    trans.location += Core::Math::SVector(0.0f, 1.0f, 0.0f) *
        (static_cast<float>(upState) * frameMoveSpeed);
}