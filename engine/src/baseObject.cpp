#include "baseObject.h"

baseObject::baseObject(std::string name) : name(name) {
}

std::string baseObject::toString() {
	return name;
}

float baseObject::HeavyComputations(int numIterations) {
	float acc = 0.0f;
	for (int i = 0; i < numIterations; i++) {
		acc += (float)(sin(i)) * (float)(cos(i));
	}
	return acc;
}

void baseObject::init() {

}

baseObject::~baseObject() {

}

void baseObject::addChild(std::shared_ptr<baseObject> child) {
	children.push_back(child);
}

void baseObject::destroy() {

}