#pragma once

#include <cstdint>

namespace Input {
	enum class EInputFlag : std::uint8_t {
		SHIFT = 1 << 0,
		CONTROL = 1 << 1,
		ALT = 1 << 2,
	};
}