#pragma once

#include "core/math/SVector.h"
#include "core/colors.h"

namespace rendering {
	struct Vertex {
		core::math::SVector position;
		core::colors::SColorRGBA color;
	};
}