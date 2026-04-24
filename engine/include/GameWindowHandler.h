#pragma once
#include <Windows.h>
#include "InputHandler.h"
#include "engineEvents.h"

class GameWindowHandler {
private:
    HWND gameWindow;
	WNDCLASSW gameWindowClass;
    static LRESULT CALLBACK handleWindowMessages(
        HWND hwnd,
        UINT msg,
        WPARAM wParam,
        LPARAM lParam
    );
    GameWindowHandler();
    ~GameWindowHandler() = default;
    MSG msg = {};
    static uint8_t PollActiveModifiers();
    
    queue<FengineEvent, 32, EOverflowResolutions::OVERWRITE_OLDEST> engineEvents;

    bool enqueueEngineEvent(const FengineEvent& event);
public:
    bool dequeueEngineEvent(FengineEvent& event);

    static GameWindowHandler& getInstance();
    void readMessages();
    bool quitRequested;
    HWND initWindow(int w, int h);
};