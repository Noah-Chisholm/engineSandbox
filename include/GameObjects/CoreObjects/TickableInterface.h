#pragma once

#include <memory>

namespace GameObjects {
    namespace CoreObjects {
        class TickableInterface {
        public:
            TickableInterface();

            virtual ~TickableInterface() = default;

            void setActive(bool newActive);

            virtual void tick(float dt);

        protected:
            bool isActive = false;
        };
    }
}