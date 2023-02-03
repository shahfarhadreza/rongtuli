#include "stdinclude.h"
#include "window.h"
#include "rongtuli.h"

Window* gWindowBeingCreated = nullptr;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

Window::Window(const std::wstring& className,
               const HWND& parentWindowHandle,
               int x, int y, int width, int height)
    : mWindowHandle(nullptr), mDoubleBufferBitmap(nullptr) {
    //const wchar_t szClassName[] = L"rongtuliAPPcanvasCLS";

    WNDCLASSEX wincl;

    wincl.hInstance = RongtuliApp::getInstance()->getInstanceHandle();
    wincl.lpszClassName = className.c_str();
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEX);

    wincl.hIcon = nullptr;
    wincl.hIconSm = nullptr;
    wincl.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
    wincl.lpszMenuName = nullptr;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (::RegisterClassEx(&wincl)) {

        gWindowBeingCreated = this;

        mWindowHandle = ::CreateWindowEx(
           0,                   /* Extended possibilites for variation */
           className.c_str(),         /* Classname */
           nullptr,       /* Title Text */
           WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, /* default window */
           x,       /* Windows decides the position */
           y,       /* where the window ends up on the screen */
           width,                 /* The programs width */
           height,                 /* and height in pixels */
           parentWindowHandle,        /* The window is a child-window to desktop */
           nullptr,                /* No menu */
           wincl.hInstance,       /* Program Instance handler */
           nullptr                 /* No Window Creation data */
        );

        gWindowBeingCreated = nullptr;

        _updateDoubleBuffer();
    }
}

Window::~Window() {
    if (mDoubleBufferBitmap != nullptr) {
        ::DeleteObject(mDoubleBufferBitmap);
    }
    if (mWindowHandle != nullptr) {
        ::DestroyWindow(mWindowHandle);
        mWindowHandle = nullptr;
    }
}

bool Window::create() {
    return true;
}

void Window::_updateDoubleBuffer() {
    RECT rcClient;
    ::GetClientRect(mWindowHandle, &rcClient);
    if (mDoubleBufferBitmap != nullptr) {
        ::DeleteObject(mDoubleBufferBitmap);
    }
    const HDC hdc = ::GetDC(mWindowHandle);
    mDoubleBufferBitmap = ::CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
    ::ReleaseDC(mWindowHandle, hdc);
}

void Window::invalidate() {
    if (mWindowHandle != nullptr) {
        ::InvalidateRect(mWindowHandle, nullptr, true);
    }
}

void Window::setSize(const Gdiplus::Size& size) {
    ::SetWindowPos(mWindowHandle, nullptr, 0, 0, size.Width, size.Height, SWP_NOMOVE);
}

Gdiplus::Size Window::getSize() {
    RECT rcClient;
    ::GetClientRect(mWindowHandle, &rcClient);
    return {rcClient.right, rcClient.bottom};
}

void Window::moveAndSize(int x, int y, int width, int height) {
    ::MoveWindow(mWindowHandle, x, y, width, height, true);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    auto wnd = RongtuliApp::getInstance()->findWindow(hwnd);

    if (wnd == nullptr) {
        if (gWindowBeingCreated) {
            wnd = gWindowBeingCreated;
            RongtuliApp::getInstance()->addWindow(hwnd, wnd);
        } else {
            return ::DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    switch (message) {
        case WM_LBUTTONDOWN: {
            auto x = LOWORD(lParam);
            auto y = HIWORD(lParam);
            MouseEventArgs me;
            me.x = float(x);
            me.y = float(y);
            wnd->mouseButtonDown(me);
            break;
        }
        case WM_LBUTTONUP: {
            auto x = LOWORD(lParam);
            auto y = HIWORD(lParam);
            MouseEventArgs me;
            me.x = float(x);
            me.y = float(y);
            wnd->mouseButtonUp(me);
            break;
        }
        case WM_MOUSEMOVE: {
            auto x = LOWORD(lParam);
            auto y = HIWORD(lParam);
            MouseEventArgs me;
            me.x = float(x);
            me.y = float(y);
            wnd->mouseMove(me);
            break;
        }
        case WM_SIZE: {
            wnd->_updateDoubleBuffer();
            break;
        }
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            ::BeginPaint(hwnd, &ps);
            const HDC hdc = ps.hdc;
            const HDC hdcBuffer = ::CreateCompatibleDC(hdc);
            ::SelectObject(hdcBuffer, wnd->mDoubleBufferBitmap);
            Gdiplus::Graphics g(hdcBuffer);
            g.Clear(Gdiplus::Color::White);

            RECT rcClient;
            ::GetClientRect(hwnd, &rcClient);

            wnd->draw(&g);

            ::BitBlt(hdc, 0,0, rcClient.right, rcClient.bottom, hdcBuffer, 0,0, SRCCOPY);
            ::DeleteDC(hdcBuffer);
            ::EndPaint(hwnd, &ps);
            break;
        }
        default:
            return ::DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}



