#pragma once

#include <Windows.h>

#include <array>
#include <bitset>
#include <vector>

#include "delegates.h"
#include "baseObject.h"
#include "inputCommon.h"
#include "circularQueue.h"

namespace input {
    class InputHandler {
    public:
        using InputEventSig = Delegate<const SInputEvent&>;

    private:
        Queue<SInputEvent, 128, EOverflowResolutions::OVERWRITE_OLDEST> queuedEvents;

        InputHandler() = default;
        ~InputHandler() = default;

        static constexpr std::size_t VK_COUNT = 256;
        static constexpr std::size_t TYPE_COUNT = static_cast<std::size_t>(static_cast<uint8_t>(EInputEventType::Count) - 1);
        static constexpr std::size_t MOUSEBTN_COUNT = static_cast<std::size_t>(static_cast<uint8_t>(EMouseInputType::Count) - 1);

        std::vector<InputEventSig> anyKeyRegistrants;
        std::array<std::vector<InputEventSig>, VK_COUNT> keyRegistrants;
        std::array<std::vector<InputEventSig>, MOUSEBTN_COUNT> mouseButtonRegistrants;
        std::vector<InputEventSig> mouseMoveRegistrants;
        std::vector<InputEventSig> mouseWheelRegistrants;

        std::bitset<VK_COUNT> keyStates; // 1 is key down
        std::bitset<MOUSEBTN_COUNT> mouseStates; // 1 is button down

        bool hasLastMousePosition = false;
        std::int32_t lastMouseX = 0;
        std::int32_t lastMouseY = 0;

    public:
        bool enqueueEvent(const SInputEvent& event);
        bool dequeueEvent(SInputEvent& event);

        static InputHandler& getInstance();

        void handleAllEvents();

        bool registerForKeyEvent(uint32_t key, InputEventSig callBack);
        bool registerForAnyKeyEvent(InputEventSig callBack);
        bool registerForMouseEvent(EMouseInputType event, InputEventSig callBack);
        bool registerForMouseMove(InputEventSig callBack);
        bool registerForMouseWheel(InputEventSig callBack);
    };
}