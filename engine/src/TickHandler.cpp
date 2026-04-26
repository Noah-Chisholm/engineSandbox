#include "TickHandler.h"
#include "logging.h"
#include "tickableInterface.h"

tickHandler::tickHandler() {
    lastFrameTime = 0.0;
    lastTickStart = std::chrono::steady_clock::now();
}

float tickHandler::tick() {
    using clock = std::chrono::steady_clock;

    auto startTime = clock::now();

    // dt between ticks
    std::chrono::duration<float> dt = startTime - lastTickStart;
    lastTickStart = startTime;

    // log("DT: {}", dt.count());

    for (auto obj : tickableObjects) {
        obj->tick(dt.count());
    }

    // how long THIS tick took to execute
    std::chrono::duration<float> frameTime = clock::now() - startTime;
    lastFrameTime = frameTime.count();

    // 
    // ("Tick length: {}", lastFrameTime);
    return lastFrameTime;
}

bool tickHandler::registerTick(std::shared_ptr<tickableInterface> registrant) {
    tickableObjects.push_back(registrant);
    return true;
}

bool tickHandler::unregisterTick(std::shared_ptr<tickableInterface> registrant) {
    int counter = 0;
    for (auto obj : tickableObjects)
        if (obj == registrant) {
            tickableObjects.erase(tickableObjects.begin() + counter);
            return true;
        } else counter++;
    return false;
}

tickHandler& tickHandler::getHandler()
{
    static tickHandler inst;
    return inst;
}
