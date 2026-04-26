#pragma once

#include <memory>

class baseObject;

class tickableInterface {
public:
    tickableInterface();

    virtual ~tickableInterface() = default;

    void setActive(bool newActive);

    virtual void tick(float dt);

protected:
    bool isActive = false;
};