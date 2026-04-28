#pragma once

#include "Core/Math/MathCommon.h"

namespace GameObjects {
	namespace CoreObjects {
		class SceneInterface {
		public:
			SceneInterface(Core::Math::STransform transformIn);
			Core::Math::STransform trans;
			Core::Math::SVector getForwardVector() const;
			Core::Math::SVector getRightVector() const;
			Core::Math::SVector getUpVector() const;
		};
	}
}