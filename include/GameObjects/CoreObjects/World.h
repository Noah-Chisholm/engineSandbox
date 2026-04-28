#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "GameObjects/CoreObjects/BaseObject.h"

namespace GameObjects {
    namespace CoreObjects {
        class World
        {
        public:
            World();
            ~World();

            template<typename T, typename... Args>
                requires std::is_base_of_v<BaseObject, T>
            std::shared_ptr<BaseObject> spawnObject(std::shared_ptr<BaseObject> parent, Args&&... args)
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
            std::vector<std::shared_ptr<BaseObject>> children;
        };
    }
}