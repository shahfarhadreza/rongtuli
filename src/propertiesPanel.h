#pragma once

#include "window.h"
#include "canvasObject.h"

class Window;
class PropertiesPanel;
class Edit;

class PropertiesPanel : public Window {
protected:
    std::shared_ptr<Edit> mEditText;
public:
    PropertiesPanel(const HWND& parentWindowhandle);
    virtual ~PropertiesPanel();
    virtual void mouseButtonDown(const MouseEventArgs& e);
    virtual void mouseButtonUp(const MouseEventArgs& e);
    virtual void mouseMove(const MouseEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g);
};


