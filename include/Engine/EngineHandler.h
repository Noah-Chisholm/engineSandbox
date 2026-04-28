#pragma once

#include <memory>
#include <type_traits>
#include <utility>

#include "Engine/TickHandler.h"
#include "Platforms/Windows32/GameWindowHandler.h"
#include "Core/CircularQueue.h"
#include "Rendering/RenderHandler.h"
#include "Engine/EngineEvents.h"
#include "GameObjects/CoreObjects/World.h"

namespace Engine {
    class EngineHandler {
    private:
        TickHandler& mainTickHandler = TickHandler::getHandler();
        Platforms::Win32::GameWindowHandler& mainGameWindow = Platforms::Win32::GameWindowHandler::getInstance();
        Input::InputHandler& mainInputHandler = Input::InputHandler::getInstance();
        Rendering::RenderHandler& mainRenderHandler = Rendering::RenderHandler::getInstance();


        void gameplayLoop();

        bool isMinimized = false;

        EngineHandler() = default;
        ~EngineHandler() = default;

        void handleEngineEvents();

        std::unique_ptr<GameObjects::CoreObjects::World> activeWorld;

        int defaultWindowW = 800;
        int defaultWindowH = 600;

    public:
        void startEngine();

        void handleResize(int w, int h, bool minimized);

        static EngineHandler& getInstance();
    };
}