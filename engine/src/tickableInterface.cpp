#include "tickableInterface.h"
#include "baseObject.h"
#include "TickHandler.h"

#include <cassert>
#include <memory>

tickableInterface::tickableInterface() = default;

void tickableInterface::setActive(bool newActive) {
    if (isActive == newActive) {
        return;
    }

    isActive = newActive;

    baseObject* owningBaseObject = dynamic_cast<baseObject*>(this);

    assert(owningBaseObject && "tickableInterface must also be part of a baseObject-derived object.");

    if (!owningBaseObject) {
        return;
    }

    std::shared_ptr<baseObject> selfBase =
        owningBaseObject->getSharedBaseObject();

    std::shared_ptr<tickableInterface> selfTickable =
        std::dynamic_pointer_cast<tickableInterface>(selfBase);

    assert(selfTickable && "baseObject was not actually tickable.");

    if (!selfTickable) {
        return;
    }

    if (isActive) {
        tickHandler::getHandler().registerTick(selfTickable);
    }
    else {
        tickHandler::getHandler().unregisterTick(selfTickable);
    }
}

void tickableInterface::tick(float dt) {
}