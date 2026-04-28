#include "input/InputHandler.h"
#include "GameWindowHandler.h"

using namespace input;

bool InputHandler::dequeueEvent(SInputEvent& event) {
    return queuedEvents.dequeue(event);
}

bool InputHandler::enqueueEvent(const SInputEvent& event) {
    return queuedEvents.enqueue(event);
}

InputHandler& InputHandler::getInstance() {
    static InputHandler inst;
    return inst;
}

void InputHandler::handleAllEvents() {
    SInputEvent eventToHandle{};

    while (dequeueEvent(eventToHandle)) {
        switch (eventToHandle.type) {
        case EInputEventType::KeyDown: {
            auto oldState = keyStates[eventToHandle.key.vk];

            if (oldState == true) {
                break;
            }

            keyStates[eventToHandle.key.vk] = true;

            std::uint8_t vk = static_cast<std::uint8_t>(eventToHandle.key.vk);
            auto& eventsToCall = keyRegistrants[vk];

            for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
                (*it)(eventToHandle);
            }

            for (auto it = anyKeyRegistrants.begin(); it != anyKeyRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case EInputEventType::KeyUp: {
            auto oldState = keyStates[eventToHandle.key.vk];

            if (oldState == false) {
                break;
            }

            keyStates[eventToHandle.key.vk] = false;

            std::uint8_t vk = static_cast<std::uint8_t>(eventToHandle.key.vk);
            auto& eventsToCall = keyRegistrants[vk];

            for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
                (*it)(eventToHandle);
            }

            for (auto it = anyKeyRegistrants.begin(); it != anyKeyRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case EInputEventType::MouseDown: {
            auto mouseIndex = static_cast<std::uint8_t>(eventToHandle.mouseBtn.button);
            auto oldState = mouseStates[mouseIndex];

            if (oldState == true) {
                break;
            }

            mouseStates[mouseIndex] = true;

            auto& eventsToCall = mouseButtonRegistrants[mouseIndex];

            for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case EInputEventType::MouseUp: {
            auto mouseIndex = static_cast<std::uint8_t>(eventToHandle.mouseBtn.button);
            auto oldState = mouseStates[mouseIndex];

            if (oldState == false) {
                break;
            }

            mouseStates[mouseIndex] = false;

            auto& eventsToCall = mouseButtonRegistrants[mouseIndex];

            for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case EInputEventType::MouseMove: {
            auto& window = GameWindowHandler::getInstance();

            if (window.mouseLocked) {
                POINT center = window.getClientCenterClientPosition();

                const std::int32_t mouseX = static_cast<std::int32_t>(eventToHandle.mouseMove.x);
                const std::int32_t mouseY = static_cast<std::int32_t>(eventToHandle.mouseMove.y);

                const std::int32_t centerX = static_cast<std::int32_t>(center.x);
                const std::int32_t centerY = static_cast<std::int32_t>(center.y);

                eventToHandle.mouseMove.deltaX = mouseX - centerX;
                eventToHandle.mouseMove.deltaY = mouseY - centerY;

                if (eventToHandle.mouseMove.deltaX == 0 &&
                    eventToHandle.mouseMove.deltaY == 0) {
                    break;
                }

                window.centerMouse(center);

                hasLastMousePosition = false;
            }
            else {
                const std::int32_t mouseX = static_cast<std::int32_t>(eventToHandle.mouseMove.x);
                const std::int32_t mouseY = static_cast<std::int32_t>(eventToHandle.mouseMove.y);

                if (!hasLastMousePosition) {
                    lastMouseX = mouseX;
                    lastMouseY = mouseY;
                    hasLastMousePosition = true;

                    eventToHandle.mouseMove.deltaX = 0;
                    eventToHandle.mouseMove.deltaY = 0;
                }
                else {
                    eventToHandle.mouseMove.deltaX = mouseX - lastMouseX;
                    eventToHandle.mouseMove.deltaY = mouseY - lastMouseY;

                    lastMouseX = mouseX;
                    lastMouseY = mouseY;
                }
            }

            for (auto it = mouseMoveRegistrants.begin(); it != mouseMoveRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case EInputEventType::MouseWheel: {
            eventToHandle.wheel.delta /= WHEEL_DELTA;

            for (auto it = mouseWheelRegistrants.begin(); it != mouseWheelRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        default: {
            break;
        }
        }
    }
}

bool InputHandler::registerForKeyEvent(uint32_t key, InputEventSig callBack) {
    keyRegistrants[key].push_back(callBack);
    return true;
}

bool InputHandler::registerForAnyKeyEvent(InputEventSig callBack) {
    anyKeyRegistrants.push_back(callBack);
    return true;
}

bool InputHandler::registerForMouseEvent(EMouseInputType event, InputEventSig callBack) {
    mouseButtonRegistrants[static_cast<uint8_t>(event)].push_back(callBack);
    return true;
}

bool InputHandler::registerForMouseMove(InputEventSig callBack) {
    mouseMoveRegistrants.push_back(callBack);
    return true;
}

bool InputHandler::registerForMouseWheel(InputEventSig callBack) {
    mouseWheelRegistrants.push_back(callBack);
    return true;
}