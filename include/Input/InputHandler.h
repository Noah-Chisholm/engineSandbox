#pragma once

#include <Windows.h>
#include <array>
#include <bitset>
#include <cstddef>
#include <vector>

#include "Core/Delegate.h"
#include "GameObjects/CoreObjects/BaseObject.h"
#include "Input/InputCommon.h"
#include "Core/CircularQueue.h"

namespace Input {
    class InputHandler {
    public:
        using InputEventSig = Core::Delegate<const SInputEvent&>;

    private:
        Core::CircularQueue<SInputEvent, 128, Core::EOverflowResolution::OVERWRITE_OLDEST> queuedEvents;

        InputHandler() = default;
        ~InputHandler() = default;

        static constexpr std::size_t VK_COUNT = 256;
        static constexpr std::size_t TYPE_COUNT = static_cast<std::size_t>(static_cast<std::size_t>(EInputEventType::Count));
        static constexpr std::size_t MOUSEBTN_COUNT = static_cast<std::size_t>(static_cast<std::size_t>(MouseInput::EMouseInputType::Count));

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

        bool registerForKeyEvent(std::uint32_t key, InputEventSig callBack);
        bool registerForAnyKeyEvent(InputEventSig callBack);
        bool registerForMouseEvent(MouseInput::EMouseInputType event, InputEventSig callBack);
        bool registerForMouseMove(InputEventSig callBack);
        bool registerForMouseWheel(InputEventSig callBack);
    };
}