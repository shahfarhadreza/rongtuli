#include "stdinclude.h"
#include "control.h"
#include "button.h"
#include "rongtuli.h"

Button::Button(const HWND& parentWindowhandle, const std::wstring& text, int x, int y, int width, int height)
: mOnClick(nullptr) {
    auto app = RongtuliApp::getInstance();
    const HINSTANCE hInstance = app->getInstanceHandle();
    mWindowHandle = ::CreateWindowExW(
           0,
           L"button",
           text.c_str(),
           WS_CHILD | WS_VISIBLE,
           x,
           y,
           width,
           height,
           parentWindowhandle,
           nullptr,
           hInstance,
           nullptr
           );
    app->addControl(mWindowHandle, this);
}

Button::~Button() {
    if (mWindowHandle != nullptr) {
        ::DestroyWindow(mWindowHandle);
        mWindowHandle = nullptr;
    }
}

void Button::WMCommand(WPARAM wParam) {
    switch (HIWORD(wParam)) {
        case BN_CLICKED:
            if (mOnClick != nullptr) {
                mOnClick();
            }
            break;

        default:
            break;
    }
}

void Button::onClick(const std::function<void()>& fn) {
    this->mOnClick = fn;
}



