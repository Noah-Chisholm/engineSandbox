#pragma once

#include "baseObject.h"
#include <vector>

class world
{
public:
	world();
	~world();

    template<typename T, typename... Args>
        requires std::is_base_of_v<baseObject, T>
    std::shared_ptr<baseObject> spawnObject(std::shared_ptr<baseObject> parent, Args&&... args)
    {
        auto obj = std::make_shared<T>(std::forward<Args>(args)...);
        if (parent) {
            parent->addChild(obj);
        }
        else {
            children.push_back(obj);
        }
        obj->init();
        return obj; // upcast is fine
    }

private:
    std::vector<std::shared_ptr<baseObject>> children;
};