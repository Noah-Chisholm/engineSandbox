#include "GameWindowHandler.h"
#include "InputHandler.h"
#include "engine.h"
#include <windowsx.h>

GameWindowHandler::GameWindowHandler() {

}

HWND GameWindowHandler::initWindow(int w, int h) {
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
        w, h, nullptr, nullptr, hInstance, nullptr);
    ShowWindow(gameWindow, SW_SHOW);
    return gameWindow;
}

void GameWindowHandler::readMessages() {
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

uint8_t GameWindowHandler::PollActiveModifiers()
{
    uint8_t mods = 0;
    if (GetKeyState(VK_SHIFT) & 0x8000) mods |= MOD_SHIFT;
    if (GetKeyState(VK_CONTROL) & 0x8000) mods |= MOD_CONTROL;
    if (GetKeyState(VK_MENU) & 0x8000) mods |= MOD_ALT;
    return mods;
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
    switch (msg) {
    case WM_CLOSE:
        GameWindowHandler::getInstance().quitRequested = true;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::KeyDown;
        event.mods = PollActiveModifiers();
        event.key.vk = static_cast<uint32_t>(wParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_KEYUP: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::KeyUp;
        event.mods = PollActiveModifiers();
        event.key.vk = static_cast<uint32_t>(wParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods = PollActiveModifiers();
        event.mouseBtn.button = input::EMouseInputTypes::Left;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_RBUTTONDOWN: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods = PollActiveModifiers();
        event.mouseBtn.button = input::EMouseInputTypes::Right;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_MBUTTONDOWN: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods = PollActiveModifiers();
        event.mouseBtn.button = input::EMouseInputTypes::Middle;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_XBUTTONDOWN: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods = PollActiveModifiers();

        WORD xbtn = GET_XBUTTON_WPARAM(wParam);
        event.mouseBtn.button =
            (xbtn == XBUTTON1)
            ? input::EMouseInputTypes::X1
            : input::EMouseInputTypes::X2;

        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_LBUTTONUP: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods = PollActiveModifiers();
        event.mouseBtn.button = input::EMouseInputTypes::Left;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_RBUTTONUP: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods = PollActiveModifiers();
        event.mouseBtn.button = input::EMouseInputTypes::Right;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_MBUTTONUP: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods = PollActiveModifiers();
        event.mouseBtn.button = input::EMouseInputTypes::Middle;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_XBUTTONUP: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods = PollActiveModifiers();

        WORD xbtn = GET_XBUTTON_WPARAM(wParam);
        event.mouseBtn.button =
            (xbtn == XBUTTON1)
            ? input::EMouseInputTypes::X1
            : input::EMouseInputTypes::X2;

        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_MOUSEMOVE: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseMove;
        event.mods = PollActiveModifiers();
        event.mouseMove.x = GET_X_LPARAM(lParam);
        event.mouseMove.y = GET_Y_LPARAM(lParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_MOUSEWHEEL: {
        input::FInputEvent event{};
        event.type = input::EInputEventType::MouseWheel;
        event.mods = PollActiveModifiers();
        event.wheel.x = GET_X_LPARAM(lParam);
        event.wheel.y = GET_Y_LPARAM(lParam);
        event.wheel.delta = GET_WHEEL_DELTA_WPARAM(wParam);

        inputHandler::getInstance().enqueueEvent(event);
        return 0;
    }
    case WM_SIZE:
    {
        UINT newWidth = LOWORD(lParam);
        UINT newHeight = HIWORD(lParam);

        bool minimized = (wParam == SIZE_MINIMIZED);
        
        FengineEvent newEvent{};
        newEvent.type = EEngineEventType::RESIZE;
        newEvent.resize = { newWidth, newHeight, minimized };
        GameWindowHandler::getInstance().enqueueEngineEvent(newEvent);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool GameWindowHandler::dequeueEngineEvent(FengineEvent& event) {
    return engineEvents.dequeue(event);
}

bool GameWindowHandler::enqueueEngineEvent(const FengineEvent& event) {
    return engineEvents.enqueue(event);
}