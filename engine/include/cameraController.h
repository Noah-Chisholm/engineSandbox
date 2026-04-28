#pragma once

#include "camera.h"
#include "input/inputHandler.h"
#include "tickableInterface.h"

class cameraController : public camera, public tickableInterface {
public:
    cameraController(name _str, transform _transform, cameraData _data);

    void handleInput(const input::SInputEvent& event);

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

    
    input::InputHandler::InputEventSig inputDel;
};