#pragma once

#include "core/math/mathCommon.h"

class sceneInterface {
public:
	sceneInterface(core::math::STransform transformIn);
	core::math::STransform trans;
	core::math::SVector getForwardVector() const;
	core::math::SVector getRightVector() const;
	core::math::SVector getUpVector() const;
};