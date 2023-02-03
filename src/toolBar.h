#pragma once

#include "window.h"
#include "canvasObject.h"
#include "tool.h"

class Window;
class ToolBar;

enum ButtonType {
    NonToggleButton,
    ToggleButton,
};

class ToolButton {
friend class ToolBar;
protected:
    std::wstring mText;
    Gdiplus::Bitmap* mImage;
    Gdiplus::RectF mRect;
    bool mHighlight;
    bool mSelected;
    std::function<void()> mOnClick;
    ButtonType mType;
public:
    ToolButton(const std::wstring& text, const std::wstring& icon, const std::function<void()>& onClick)
        : mText(text), mHighlight(false), mSelected(false), mOnClick(onClick), mType(ToggleButton) {
        mImage = Gdiplus::Bitmap::FromFile(icon.c_str());
    }
    const std::wstring& getText() const {
        return mText;
    }
    Gdiplus::Bitmap* getImage() {
        return mImage;
    }
};

class ToolBar : public Window {
protected:
    std::vector<std::shared_ptr<ToolButton>> mToolButtonList;
public:
    ToolBar(const HWND& parentWindowhandle);
    virtual ~ToolBar();
    void setDefaultTool(ToolButton* tool);
    ToolButton* addButton(const std::wstring& text, const std::wstring& icon, const std::function<void()>& onClick = nullptr);
    virtual void mouseButtonDown(const MouseEventArgs& e);
    virtual void mouseButtonUp(const MouseEventArgs& e);
    virtual void mouseMove(const MouseEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g);
};


