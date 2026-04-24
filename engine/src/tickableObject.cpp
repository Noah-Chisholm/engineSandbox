#include "tickableObject.h"
#include "TickHandler.h"
#include "memory.h"


tickableObject::tickableObject(std::string name, bool isActive) : baseObject(name), isActive(!isActive) {

}

void tickableObject::setActive(bool newActive) {
	if (isActive != newActive) {
		isActive = newActive;
		if(isActive)
			tickHandler::getHandler().registerTick(std::static_pointer_cast<tickableObject>(shared_from_this()));
		else
			tickHandler::getHandler().unregisterTick(std::static_pointer_cast<tickableObject>(shared_from_this()));
	}
}

void tickableObject::tick(float dt)
{
	HeavyComputations(1000000);
}

void tickableObject::init() {
	setActive(!isActive);
}

tickableObject::~tickableObject() {
}

void tickableObject::destroy() {
	setActive(false);
}