#include "GameObjects/CoreObjects/BaseObject.h"

using namespace GameObjects::CoreObjects;

BaseObject::BaseObject(Core::Name _str) : _name(_str) {
}

std::string BaseObject::toString() {
	return _name.getString();
}

void BaseObject::init() {

}

BaseObject::~BaseObject() {

}

void BaseObject::addChild(std::shared_ptr<BaseObject> child) {
	children.push_back(child);
}

void BaseObject::destroy() {

}