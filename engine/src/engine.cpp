#include "engine.h"

void engine::gameplayLoop() {
    while (!mainGameWindow.quitRequested) {
        mainGameWindow.readMessages();
        mainTickHandler.tick();
        mainInputHandler.handleAllEvents();
        handleEngineEvents();
    }
    activeWorld.reset();
}
    
void engine::startEngine() {
    activeWorld = std::make_unique<world>();
    mainRenderHandler.init(defaultWindowW, defaultWindowH, mainGameWindow.initWindow(defaultWindowW, defaultWindowH));
    activeWorld->spawnObject<inputTester>(nullptr, "Tester");
    gameplayLoop();
}

void engine::handleResize(int w, int h, bool minimized) {
    isMinimized = minimized;
}

engine& engine::getInstance() {
    static engine inst;
    return inst;
}

void engine::handleEngineEvents() {
    FengineEvent curEvent{};
    while (mainGameWindow.dequeueEngineEvent(curEvent) ) {
        switch (curEvent.type)
        {
        case EEngineEventType::RESIZE:
            handleResize(curEvent.resize.newW, curEvent.resize.newH, curEvent.resize.newMin);
            break;
        default:
            break;
        }
    }
}
