#pragma once

#include "rendering.h"

class sceneInterface {
public:
	sceneInterface(transform transformIn);
	transform trans;
	fVector getForwardVector() const;
	fVector getRightVector() const;
	fVector getUpVector() const;
};