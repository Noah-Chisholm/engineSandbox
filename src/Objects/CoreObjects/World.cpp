#include "GameObjects/CoreObjects/World.h"

using namespace GameObjects::CoreObjects;

World::World()
{
}

World::~World() {
	for (auto child : children) {
		child->destroy();
	}
}