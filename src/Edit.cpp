#include "stdinclude.h"
#include "control.h"
#include "edit.h"
#include "rongtuli.h"

WNDPROC OldEditProc;
LRESULT CALLBACK EditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

Edit::Edit(const HWND& parentWindowhandle, const std::wstring& text, int x, int y, int width, int height)
: mOnEnter(nullptr) {
    auto app = RongtuliApp::getInstance();
    const HINSTANCE hInstance = app->getInstanceHandle();
    mWindowHandle = ::CreateWindowExW(
           0,
           L"edit",
           text.c_str(),
           WS_BORDER | WS_CHILD | WS_VISIBLE,
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
    OldEditProc = (WNDPROC)::SetWindowLongPtr(mWindowHandle, GWLP_WNDPROC, (LONG_PTR)EditProc);
}

Edit::~Edit() {
    if (mWindowHandle != nullptr) {
        ::DestroyWindow(mWindowHandle);
        mWindowHandle = nullptr;
    }
}

void Edit::WMCommand(WPARAM wParam) {

}

void Edit::keyDown(int key) {
    if (key == VK_RETURN) {
        if (mOnEnter != nullptr) {
            mOnEnter();
        }
    }
}

std::wstring Edit::getText() {
    int nLnWithNul = ::GetWindowTextLengthW(mWindowHandle) + 1;
	std::wstring text(nLnWithNul, '\0'); // bit of a shorthand for the resize call
	int nCopiedLn = ::GetWindowTextW(mWindowHandle, &text[0], nLnWithNul);
	text.resize(nCopiedLn);
	return text;
}

LRESULT CALLBACK EditProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto app = RongtuliApp::getInstance();
    switch (msg)
    {
    case WM_KEYDOWN:
         switch (wParam)
         {
          case VK_RETURN: {
            //Do your stuff
            Control* control = app->findControl(hwnd);
            if (control != nullptr) {
                control->keyDown(VK_RETURN);
            }
            break;
          }
         }
    default:
         return ::CallWindowProc(OldEditProc, hwnd, msg, wParam, lParam);
    }
    return 0;
}





