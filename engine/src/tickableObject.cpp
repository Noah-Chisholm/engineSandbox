#include "tickableObject.h"
#include "globalFunctions.h"
#include "TickHandler.h"



tickableObject::tickableObject(std::string name) : baseObject(name) {
	tickHandler::getHandler().registerTick(this);
}

void tickableObject::tick(float dt)
{
	HeavyComputations(1000000);
}
