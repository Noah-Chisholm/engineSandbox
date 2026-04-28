#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "TickHandler.h"
#include "GameWindowHandler.h"
#include "inputTester.h"
#include "circularQueue.h"
#include "renderHandler.h"
#include "engineEvents.h"
#include "world.h"

class engine {
private:
    tickHandler& mainTickHandler = tickHandler::getHandler();
    GameWindowHandler& mainGameWindow = GameWindowHandler::getInstance();
    input::InputHandler& mainInputHandler = input::InputHandler::getInstance();
    renderHandler& mainRenderHandler = renderHandler::getInstance();


    void gameplayLoop();

    bool isMinimized = false;

    engine() = default;
    ~engine() = default;
    
    void handleEngineEvents();

    std::unique_ptr<world> activeWorld;

    int defaultWindowW = 800;
    int defaultWindowH = 600;

public:
    void startEngine();

    void handleResize(int w, int h, bool minimized);

    static engine& getInstance();
};
