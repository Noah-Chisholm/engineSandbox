#pragma once

#include "Input/MouseInput.h"
#include "Input/KeyboardInput.h"
#include "Input/InputFlags.h"
#include "Core/FlagSet.h"

namespace Input {
    enum class EInputEventType : std::uint8_t {
        KeyDown,
        KeyUp,
        MouseDown,
        MouseUp,
        MouseMove,
        MouseWheel,

        Count
    };

    struct SInputEvent {
        EInputEventType type{};
        Core::FlagSet<EInputFlag> mods;

        union {
            KeyboardInput::SKeyEvent key;
            MouseInput::SMouseBtnEvent mouseBtn;
            MouseInput::SMouseMoveEvent mouseMove;
            MouseInput::SMouseWheelEvent wheel;
        };
    };
}