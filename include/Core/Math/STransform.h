#pragma once

#include "Core/Math/SVector.h"
#include "Core/Math/SRotator.h"

namespace Core {
	namespace Math {
		struct STransform {
			SVector location = SVector(0.0f);
			SRotator rotation = SRotator(0.0f);
			SVector scale = SVector(1.0f);
		};
	}
}