#pragma once

#include "Core/Math/SVector.h"
#include "Core/Colors.h"

namespace Rendering {
	struct SVertex {
		Core::Math::SVector position;
		Core::Colors::SColorRGBA color;
	};
}