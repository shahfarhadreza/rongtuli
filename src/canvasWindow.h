#pragma once

#include "window.h"
#include "canvasObject.h"
#include "tool.h"

class Window;

class CanvasWindow : public Window {
protected:
    std::vector<std::shared_ptr<CanvasObject>> mObjectList;
public:
    CanvasWindow(const HWND& parentWindowhandle);
    virtual ~CanvasWindow();
    void insertObject(const std::shared_ptr<CanvasObject>& object);
    const std::vector<std::shared_ptr<CanvasObject>>& getObjectList() const;
    bool getSelectedObjects(std::vector<std::shared_ptr<CanvasObject>>& list);
    void deleteSelectedObject();
    virtual void mouseButtonDown(const MouseEventArgs& e);
    virtual void mouseButtonUp(const MouseEventArgs& e);
    virtual void mouseMove(const MouseEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g);
};

