#pragma once

#include "window.h"
#include "canvasObject.h"

class Window;
class ColorPanel;

class ColorButton {
friend class ColorPanel;
protected:
    Gdiplus::Color mColor;
    Gdiplus::RectF mRect;
    bool mHighlight;
    bool mSelected;
public:
    ColorButton(const Gdiplus::Color& color)
        : mColor(color), mHighlight(false), mSelected(false) {

    }
    const Gdiplus::Color& getColor() const {
        return mColor;
    }
};

class ColorPanel : public Window {
protected:
    std::vector<std::shared_ptr<ColorButton>> mColorButtonList;
public:
    ColorPanel(const HWND& parentWindowhandle);
    virtual ~ColorPanel();
    ColorButton* addColor(const Gdiplus::Color& color);
    virtual void mouseButtonDown(const MouseEventArgs& e);
    virtual void mouseButtonUp(const MouseEventArgs& e);
    virtual void mouseMove(const MouseEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g);
};


