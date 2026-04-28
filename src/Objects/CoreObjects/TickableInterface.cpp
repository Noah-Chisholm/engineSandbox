#include <cassert>
#include <memory>

#include "GameObjects/CoreObjects/TickableInterface.h"
#include "GameObjects/CoreObjects/BaseObject.h"
#include "Engine/TickHandler.h"

using namespace GameObjects::CoreObjects;

TickableInterface::TickableInterface() = default;

void TickableInterface::setActive(bool newActive) {
    if (isActive == newActive) {
        return;
    }

    isActive = newActive;

    BaseObject* owningBaseObject = dynamic_cast<BaseObject*>(this);

    assert(owningBaseObject && "tickableInterface must also be part of a baseObject-derived object.");

    if (!owningBaseObject) {
        return;
    }

    std::shared_ptr<BaseObject> selfBase =
        owningBaseObject->getSharedBaseObject();

    std::shared_ptr<TickableInterface> selfTickable =
        std::dynamic_pointer_cast<TickableInterface>(selfBase);

    assert(selfTickable && "baseObject was not actually tickable.");

    if (!selfTickable) {
        return;
    }

    if (isActive) {
        Engine::TickHandler::getHandler().registerTick(selfTickable);
    }
    else {
        Engine::TickHandler::getHandler().unregisterTick(selfTickable);
    }
}

void TickableInterface::tick(float dt) {
}