#include "GameWindowHandler.h"
#include "input/inputHandler.h"
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

    // Important:
    // Do NOT write "HWND gameWindow =" here.
    // That creates a new local variable and leaves the member gameWindow unchanged.
    gameWindow = CreateWindowExW(
        0,
        L"gameWindowClass",
        L"gameWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        w,
        h,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    ShowWindow(gameWindow, SW_SHOW);

    // This makes your current testing behavior simple:
    // when the window opens, the mouse is locked.
    //
    // If this becomes annoying later, remove this line and only lock on WM_SETFOCUS
    // or right mouse down.
    lockMouseToWindowCenter();

    return gameWindow;
}

void GameWindowHandler::readMessages() {
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

uint8_t GameWindowHandler::PollActiveModifiers() {
    uint8_t mods = 0;

    if (GetKeyState(VK_SHIFT) & 0x8000) {
        mods |= MOD_SHIFT;
    }

    if (GetKeyState(VK_CONTROL) & 0x8000) {
        mods |= MOD_CONTROL;
    }

    if (GetKeyState(VK_MENU) & 0x8000) {
        mods |= MOD_ALT;
    }

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
    case WM_SETFOCUS: {
        GameWindowHandler::getInstance().lockMouseToWindowCenter();
        return 0;
    }

    case WM_KILLFOCUS: {
        GameWindowHandler::getInstance().unlockMouse();
        return 0;
    }

    case WM_CLOSE: {
        GameWindowHandler::getInstance().unlockMouse();
        GameWindowHandler::getInstance().quitRequested = true;
        return 0;
    }

    case WM_DESTROY: {
        GameWindowHandler::getInstance().unlockMouse();
        PostQuitMessage(0);
        return 0;
    }

    case WM_KEYDOWN: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::KeyDown;
        event.mods.setRawValue(PollActiveModifiers());
        event.key.vk = static_cast<uint32_t>(wParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_KEYUP: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::KeyUp;
        event.mods.setRawValue(PollActiveModifiers());
        event.key.vk = static_cast<uint32_t>(wParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods.setRawValue(PollActiveModifiers());
        event.mouseBtn.button = input::EMouseInputType::Left;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_RBUTTONDOWN: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods.setRawValue(PollActiveModifiers());
        event.mouseBtn.button = input::EMouseInputType::Right;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_MBUTTONDOWN: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods.setRawValue(PollActiveModifiers());
        event.mouseBtn.button = input::EMouseInputType::Middle;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_XBUTTONDOWN: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseDown;
        event.mods.setRawValue(PollActiveModifiers());

        WORD xbtn = GET_XBUTTON_WPARAM(wParam);

        event.mouseBtn.button =
            (xbtn == XBUTTON1)
            ? input::EMouseInputType::X1
            : input::EMouseInputType::X2;

        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_LBUTTONUP: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods.setRawValue(PollActiveModifiers());
        event.mouseBtn.button = input::EMouseInputType::Left;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_RBUTTONUP: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods.setRawValue(PollActiveModifiers());
        event.mouseBtn.button = input::EMouseInputType::Right;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_MBUTTONUP: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods.setRawValue(PollActiveModifiers());
        event.mouseBtn.button = input::EMouseInputType::Middle;
        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_XBUTTONUP: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseUp;
        event.mods.setRawValue(PollActiveModifiers());

        WORD xbtn = GET_XBUTTON_WPARAM(wParam);

        event.mouseBtn.button =
            (xbtn == XBUTTON1)
            ? input::EMouseInputType::X1
            : input::EMouseInputType::X2;

        event.mouseBtn.x = GET_X_LPARAM(lParam);
        event.mouseBtn.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_MOUSEMOVE: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseMove;
        event.mods.setRawValue(PollActiveModifiers());

        // WM_MOUSEMOVE lParam contains client-area coordinates.
        event.mouseMove.x = GET_X_LPARAM(lParam);
        event.mouseMove.y = GET_Y_LPARAM(lParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_MOUSEWHEEL: {
        input::SInputEvent event{};
        event.type = input::EInputEventType::MouseWheel;
        event.mods.setRawValue(PollActiveModifiers());

        // WM_MOUSEWHEEL lParam is screen-space position.
        event.wheel.x = GET_X_LPARAM(lParam);
        event.wheel.y = GET_Y_LPARAM(lParam);
        event.wheel.delta = GET_WHEEL_DELTA_WPARAM(wParam);

        input::InputHandler::getInstance().enqueueEvent(event);
        return 0;
    }

    case WM_SIZE: {
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

POINT GameWindowHandler::getClientCenterClientPosition() const {
    RECT clientRect{};
    GetClientRect(gameWindow, &clientRect);

    POINT center{};
    center.x = (clientRect.right - clientRect.left) / 2;
    center.y = (clientRect.bottom - clientRect.top) / 2;

    return center;
}

POINT GameWindowHandler::getClientCenterScreenPosition() const {
    POINT center = getClientCenterClientPosition();

    ClientToScreen(gameWindow, &center);

    return center;
}

void GameWindowHandler::centerMouse(POINT clientPosition) {
    if (!gameWindow) {
        return;
    }

    POINT screenPosition = clientPosition;

    ClientToScreen(gameWindow, &screenPosition);
    SetCursorPos(screenPosition.x, screenPosition.y);
}

void GameWindowHandler::lockMouseToWindowCenter() {
    if (!gameWindow) {
        return;
    }

    if (mouseLocked) {
        centerMouse(getClientCenterClientPosition());
        return;
    }

    mouseLocked = true;

    SetCapture(gameWindow);

    RECT clientRect{};
    GetClientRect(gameWindow, &clientRect);

    POINT topLeft{ clientRect.left, clientRect.top };
    POINT bottomRight{ clientRect.right, clientRect.bottom };

    ClientToScreen(gameWindow, &topLeft);
    ClientToScreen(gameWindow, &bottomRight);

    RECT clipRect{};
    clipRect.left = topLeft.x;
    clipRect.top = topLeft.y;
    clipRect.right = bottomRight.x;
    clipRect.bottom = bottomRight.y;

    ClipCursor(&clipRect);

    centerMouse(getClientCenterClientPosition());

    ShowCursor(FALSE);
}

void GameWindowHandler::unlockMouse() {
    if (!mouseLocked) {
        return;
    }

    mouseLocked = false;

    ClipCursor(nullptr);
    ReleaseCapture();

    ShowCursor(TRUE);
}