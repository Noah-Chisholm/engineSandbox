#include "baseObject.h"

baseObject::baseObject(std::string name) : name(name) {
}

std::string baseObject::toString() {
	return name;
}