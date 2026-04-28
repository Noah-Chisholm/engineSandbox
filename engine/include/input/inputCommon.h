#pragma once

#include "input/inputShared.h"
#include "input/mouseInput.h"
#include "input/keyboardInput.h"
#include "input/inputFlags.h"
#include "core/flags.h"

namespace input {
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
        FlagSet<EInputFlag> mods;

        union {
            SKeyEvent key;
            SMouseBtnEvent mouseBtn;
            SMouseMoveEvent mouseMove;
            SMouseWheelEvent wheel;
        };
    };
}