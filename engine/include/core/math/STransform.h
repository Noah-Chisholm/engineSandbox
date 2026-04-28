#pragma once

#include "core/math/SVector.h"
#include "core/math/SRotator.h"

namespace core {
	namespace math {
		struct STransform {
			math::SVector location = SVector(0.0f);
			math::SRotator rotation = SRotator(0.0f);
			math::SVector scale = SVector(1.0f);
		};
	}
}