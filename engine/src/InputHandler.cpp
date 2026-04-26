#include "InputHandler.h"
#include "GameWindowHandler.h"

using namespace input;

bool inputHandler::dequeueEvent(FInputEvent& event) {
    return queuedEvents.dequeue(event);
}

bool inputHandler::enqueueEvent(const FInputEvent& event) {
    return queuedEvents.enqueue(event);
}

inputHandler& inputHandler::getInstance() {
    static inputHandler inst;
    return inst;
}

void inputHandler::handleAllEvents() {
    FInputEvent eventToHandle{};

    while (dequeueEvent(eventToHandle)) {
        switch (eventToHandle.type) {
        case input::EInputEventType::Beg: {
            break;
        }

        case input::EInputEventType::KeyDown: {
            auto oldState = keyStates[eventToHandle.key.vk];

            if (oldState == true) {
                break;
            }

            keyStates[eventToHandle.key.vk] = true;

            uint8_t vk = static_cast<uint8_t>(eventToHandle.key.vk);
            auto& eventsToCall = keyRegistrants[vk];

            for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
                (*it)(eventToHandle);
            }

            for (auto it = anyKeyRegistrants.begin(); it != anyKeyRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case input::EInputEventType::KeyUp: {
            auto oldState = keyStates[eventToHandle.key.vk];

            if (oldState == false) {
                break;
            }

            keyStates[eventToHandle.key.vk] = false;

            uint8_t vk = static_cast<uint8_t>(eventToHandle.key.vk);
            auto& eventsToCall = keyRegistrants[vk];

            for (auto it = eventsToCall.begin(); it != eventsToCall.end(); it++) {
                (*it)(eventToHandle);
            }

            for (auto it = anyKeyRegistrants.begin(); it != anyKeyRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case input::EInputEventType::MouseDown: {
            auto mouseIndex = static_cast<uint8_t>(eventToHandle.mouseBtn.button);
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

        case input::EInputEventType::MouseUp: {
            auto mouseIndex = static_cast<uint8_t>(eventToHandle.mouseBtn.button);
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

        case input::EInputEventType::MouseMove: {
            auto& window = GameWindowHandler::getInstance();

            if (window.mouseLocked) {
                POINT center = window.getClientCenterClientPosition();

                const int32_t mouseX = static_cast<int32_t>(eventToHandle.mouseMove.x);
                const int32_t mouseY = static_cast<int32_t>(eventToHandle.mouseMove.y);

                const int32_t centerX = static_cast<int32_t>(center.x);
                const int32_t centerY = static_cast<int32_t>(center.y);

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
                const int32_t mouseX = static_cast<int32_t>(eventToHandle.mouseMove.x);
                const int32_t mouseY = static_cast<int32_t>(eventToHandle.mouseMove.y);

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

        case input::EInputEventType::MouseWheel: {
            eventToHandle.wheel.delta /= WHEEL_DELTA;

            for (auto it = mouseWheelRegistrants.begin(); it != mouseWheelRegistrants.end(); it++) {
                (*it)(eventToHandle);
            }

            break;
        }

        case input::EInputEventType::End: {
            break;
        }

        default: {
            break;
        }
        }
    }
}

bool inputHandler::registerForKeyEvent(uint32_t key, inputEventSig callBack) {
    keyRegistrants[key].push_back(callBack);
    return true;
}

bool inputHandler::registerForAnyKeyEvent(inputEventSig callBack) {
    anyKeyRegistrants.push_back(callBack);
    return true;
}

bool inputHandler::registerForMouseEvent(EMouseInputTypes event, inputEventSig callBack) {
    mouseButtonRegistrants[static_cast<uint8_t>(event)].push_back(callBack);
    return true;
}

bool inputHandler::registerForMouseMove(inputEventSig callBack) {
    mouseMoveRegistrants.push_back(callBack);
    return true;
}

bool inputHandler::registerForMouseWheel(inputEventSig callBack) {
    mouseWheelRegistrants.push_back(callBack);
    return true;
}