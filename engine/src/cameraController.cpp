#include "cameraController.h"
#include <algorithm>
#include "logging.h"

cameraController::cameraController(name _str, transform _transform, cameraData _data) : camera(_str, _transform, _data) {

}

void cameraController::handleInput(const FInputEvent& event) {
    if (event.type == input::EInputEventType::KeyDown ||
        event.type == input::EInputEventType::KeyUp) {

        const bool isDown = event.type == input::EInputEventType::KeyDown;

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
    else if (event.type == input::EInputEventType::MouseWheel) {
        float speedIncrease = accel;

        const bool shiftHeld =
            (event.mods & input::EInputFlags::SHIFT) != 0;

        const bool controlHeld =
            (event.mods & input::EInputFlags::CONTROL) != 0;

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

        logging::log("movespeed: {}", moveSpeed);
    }
    else if (event.type == input::EInputEventType::MouseMove) {
        logging::log("deltaX: {}, deltaY: {}", event.mouseMove.deltaX, event.mouseMove.deltaY);
        yawRadians += event.mouseMove.deltaX * mouseSensitivity;
        pitchRadians += event.mouseMove.deltaY * mouseSensitivity;

        const float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;

        pitchRadians = std::clamp(
            pitchRadians,
            -pitchLimit,
            pitchLimit
        );
        logging::log("rotation: {}\n", trans.rotation);
        trans.rotation.x = pitchRadians;
        trans.rotation.y = yawRadians;
        trans.rotation.z = 0.0f;
    }
}

void cameraController::init() {
	inputDel = inputHandler::inputEventSig::Bind<cameraController, &cameraController::handleInput>(this);
	inputHandler::getInstance().registerForKeyEvent('W', inputDel);
	inputHandler::getInstance().registerForKeyEvent('A', inputDel);
	inputHandler::getInstance().registerForKeyEvent('S', inputDel);
	inputHandler::getInstance().registerForKeyEvent('D', inputDel);
	inputHandler::getInstance().registerForMouseWheel(inputDel);
	inputHandler::getInstance().registerForMouseMove(inputDel);
	setActive(true);
}

void cameraController::tick(float dt) {
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

    trans.location += fVector(0.0f, 1.0f, 0.0f) *
        (static_cast<float>(upState) * frameMoveSpeed);
}