#pragma once
#include "baseObject.h"

class tickableObject : private baseObject {
public:
	tickableObject(std::string name);
	void tick(float dt);
private:
protected:
};