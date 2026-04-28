#pragma once

#include <Windows.h>
#include "input/inputHandler.h"
#include "engineEvents.h"

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

    static uint8_t PollActiveModifiers();

    Queue<FengineEvent, 32, EOverflowResolutions::OVERWRITE_OLDEST> engineEvents;

    bool enqueueEngineEvent(const FengineEvent& event);

public:
    bool dequeueEngineEvent(FengineEvent& event);

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