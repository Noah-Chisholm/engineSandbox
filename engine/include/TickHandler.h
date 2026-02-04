#pragma once
#include <chrono>
#include <vector>

class tickableObject;


class tickHandler {
public:
    bool registerTick(tickableObject* registrant);
    bool unregisterTick(tickableObject* registrant);
    static tickHandler& getHandler();
    float tick();

private:
    tickHandler();
    ~tickHandler() = default;

    std::chrono::time_point<std::chrono::steady_clock> lastTickStart;
    float lastFrameTime;
    std::vector<tickableObject*> tickableObjects;
};