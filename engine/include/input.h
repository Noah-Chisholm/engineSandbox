#pragma once

#include <stdint.h>

namespace input {
	enum class EInputEventType : uint8_t {
		Beg,
		KeyDown, KeyUp,
		MouseDown, MouseUp,
		MouseMove,
		MouseWheel,
		End
	};

	enum EInputFlags : uint8_t {
		SHIFT = 1 << 0,
		CONTROL = 1 << 1,
		ALT = 1 << 2,
	};

	enum class EMouseInputTypes : uint8_t { Beg, Left, Right, Middle, X1, X2, End };

	struct FInputEvent {
		EInputEventType type{};
		uint8_t mods = 0;

		union {
			struct { uint32_t vk; } key;                 // KeyDown/Up
			struct { EMouseInputTypes button; int32_t x, y; } mouseBtn; // MouseDown/Up
			struct { int32_t x, y; int32_t deltaX, deltaY; } mouseMove;           // MouseMove
			struct { int32_t delta; int32_t x, y; } wheel;// MouseWheel
		};
	};
};