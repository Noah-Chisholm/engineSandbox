#pragma once
#include <chrono>
#include <vector>

class tickableInterface;

class tickHandler {
public:
    bool registerTick(std::shared_ptr<tickableInterface> registrant);
    bool unregisterTick(std::shared_ptr<tickableInterface> registrant);
    static tickHandler& getHandler();
    float tick();

private:
    tickHandler();
    ~tickHandler() = default;

    std::chrono::time_point<std::chrono::steady_clock> lastTickStart;
    float lastFrameTime;
    std::vector<std::shared_ptr<tickableInterface>> tickableObjects;
};