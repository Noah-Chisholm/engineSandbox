#include "world.h"

world::world()
{
}

world::~world() {
	for (auto child : children) {
		child->destroy();
	}
}