#pragma once

#include "control.h"

class Button : public Control {
protected:
    std::function<void()> mOnClick;
public:
    Button(const HWND& parentWindowhandle, const std::wstring& text, int x, int y, int width, int height);
    virtual ~Button();
    virtual void WMCommand(WPARAM wParam);
    void onClick(const std::function<void()>& fn);
};

