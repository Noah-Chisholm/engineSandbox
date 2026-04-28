#pragma once

#include <chrono>
#include <vector>
#include <memory>

#include "GameObjects/CoreObjects/TickableInterface.h"

namespace Engine {
    class TickHandler {
    public:
        bool registerTick(std::shared_ptr<GameObjects::CoreObjects::TickableInterface> registrant);
        bool unregisterTick(std::shared_ptr<GameObjects::CoreObjects::TickableInterface> registrant);
        static TickHandler& getHandler();
        float tick();

    private:
        TickHandler();
        ~TickHandler() = default;

        std::chrono::time_point<std::chrono::steady_clock> lastTickStart;
        float lastFrameTime;
        std::vector<std::shared_ptr<GameObjects::CoreObjects::TickableInterface>> tickableObjects;
    };
}