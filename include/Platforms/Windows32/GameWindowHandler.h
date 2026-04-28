#pragma once

#include <Windows.h>

#include "Input/InputHandler.h"
#include "Engine/EngineEvents.h"

namespace Platforms {
    namespace Win32 {
        class GameWindowHandler {
        private:
            HWND gameWindow = nullptr;
            WNDCLASSW gameWindowClass{};

            static LRESULT CALLBACK handleWindowMessages(
                HWND hwnd,
                UINT msg,
                WPARAM wParam,
                LPARAM lParam
            );

            GameWindowHandler();
            ~GameWindowHandler() = default;

            MSG msg = {};

            static std::uint8_t PollActiveModifiers();

            Core::CircularQueue<Engine::SEngineEvent, 32, Core::EOverflowResolution::OVERWRITE_OLDEST> engineEvents;

            bool enqueueEngineEvent(const Engine::SEngineEvent& event);

        public:
            bool dequeueEngineEvent(Engine::SEngineEvent& event);

            static GameWindowHandler& getInstance();

            void readMessages();

            bool quitRequested = false;

            HWND initWindow(int w, int h);

            POINT getClientCenterClientPosition() const;
            POINT getClientCenterScreenPosition() const;

            void centerMouse(POINT clientPosition);

            void lockMouseToWindowCenter();
            void unlockMouse();

            bool mouseLocked = false;
        };
    }
}