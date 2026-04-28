#pragma once

#include <cstdint>

namespace Input {
	namespace MouseInput {
		enum class EMouseInputType : std::uint8_t { Left, Right, Middle, X1, X2, Count };

		struct SMouseMoveEvent { std::int32_t x, y; std::int32_t deltaX, deltaY; };

		struct SMouseBtnEvent { EMouseInputType button; std::int32_t x, y; };

		struct SMouseWheelEvent { std::int32_t delta; std::int32_t x, y; };
	}
}