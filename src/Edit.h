#pragma once

#include "control.h"

class Edit : public Control {
protected:
    std::function<void()> mOnEnter;
public:
    Edit(const HWND& parentWindowhandle, const std::wstring& text, int x, int y, int width, int height);
    virtual ~Edit();
    virtual void WMCommand(WPARAM wParam);
    virtual void keyDown(int key);
    void onEnter(const std::function<void()>& fn) {
        mOnEnter = fn;
    }
    std::wstring getText();
};

