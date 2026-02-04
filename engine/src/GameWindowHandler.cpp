#include "GameWindowHandler.h"
#include "InputHandler.h"

GameWindowHandler::GameWindowHandler() {
    quitRequested = false;
    gameWindowClass = {};
    gameWindowClass.lpfnWndProc = GameWindowHandler::handleWindowMessages;
    auto hInstance = GetModuleHandleW(nullptr);
    gameWindowClass.hInstance = hInstance;
    gameWindowClass.lpszClassName = L"gameWindowClass";

    RegisterClassW(&gameWindowClass);

    HWND gameWindow = CreateWindowExW(
        0, L"gameWindowClass", L"gameWindow",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(gameWindow, SW_SHOW);
}

void GameWindowHandler::readMessages() {
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

GameWindowHandler& GameWindowHandler::getInstance() {
	static GameWindowHandler inst;
	return inst;
}

LRESULT CALLBACK GameWindowHandler::handleWindowMessages(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    bool up = false;
    switch (msg) {
    case WM_CLOSE:
        GameWindowHandler::getInstance().quitRequested = true;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYUP:
        up = true;
    case WM_KEYDOWN:
        uint8_t mods = 0;
        if (GetKeyState(VK_SHIFT) & 0x8000)   mods |= input::EKeyFlags::SHIFT;
        if (GetKeyState(VK_CONTROL) & 0x8000) mods |= input::EKeyFlags::CONTROL;
        if (GetKeyState(VK_MENU) & 0x8000)    mods |= input::EKeyFlags::ALT;
        if (up) mods |= input::EKeyFlags::UP;
        
        input::inputHandler::getInstance().enqueueEvent(input::FkeyEvent(input::Key, 0, wParam, mods));
        return 0;
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        up = true;
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        uint8_t mods = 0;
        if (GetKeyState(VK_SHIFT) & 0x8000)   mods |= input::EKeyFlags::SHIFT;
        if (GetKeyState(VK_CONTROL) & 0x8000) mods |= input::EKeyFlags::CONTROL;
        if (GetKeyState(VK_MENU) & 0x8000)    mods |= input::EKeyFlags::ALT;
        if (up) mods |= input::EKeyFlags::UP;

        input::inputHandler::getInstance().enqueueEvent(input::FkeyEvent(input::MouseButton, lParam, wParam, mods));
        return 0;
    case WM_VSCROLL:
        uint8_t mods = 0;
        if (GetKeyState(VK_SHIFT) & 0x8000)   mods |= input::EKeyFlags::SHIFT;
        if (GetKeyState(VK_CONTROL) & 0x8000) mods |= input::EKeyFlags::CONTROL;
        if (GetKeyState(VK_MENU) & 0x8000)    mods |= input::EKeyFlags::ALT;

        input::inputHandler::getInstance().enqueueEvent(input::FkeyEvent(input::MouseButton, lParam, wParam, mods));
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}