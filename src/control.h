#pragma once

class Control {
protected:
    HWND mWindowHandle;
public:
    Control() : mWindowHandle(nullptr) { }
    virtual ~Control() { }
    virtual void WMCommand(WPARAM wParam) { }
    virtual void keyDown(int key) { }
};

