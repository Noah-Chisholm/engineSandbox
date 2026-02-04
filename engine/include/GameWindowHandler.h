#pragma once
#include <Windows.h>
#include "InputHandler.h"

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
public:
    static GameWindowHandler& getInstance();
    void readMessages();
    bool quitRequested;
};