#include "stdinclude.h"
#include "canvasWindow.h"
#include "mainWindow.h"
#include "rongtuli.h"

CanvasWindow::CanvasWindow(const HWND& parentWindowhandle)
    : Window(L"canvasClass", parentWindowhandle,
             40,
            100,
            1024-60,
            720-120) {

}

CanvasWindow::~CanvasWindow() {

}

void CanvasWindow::insertObject(const std::shared_ptr<CanvasObject>& object) {
    mObjectList.push_back(object);
    invalidate();
}

bool CanvasWindow::getSelectedObjects(std::vector<std::shared_ptr<CanvasObject>>& list) {
    for (auto& obj : mObjectList) {
        if (obj->isSelected()) {
            list.push_back(obj);
        }
    }
    return true;
}

const std::vector<std::shared_ptr<CanvasObject>>& CanvasWindow::getObjectList() const {
    return mObjectList;
}

void CanvasWindow::deleteSelectedObject() {
    auto it = mObjectList.begin();
    while (it != mObjectList.end()) {
        if ((*it)->isSelected()) {
            it = mObjectList.erase(it);
        }
        else {
            ++it;
        }
    }
    invalidate();
}

void CanvasWindow::mouseButtonDown(const MouseEventArgs& e) {
    auto tool = RongtuliApp::getInstance()->getCurrentTool();
    if (tool != nullptr) {
        ToolEventArgs tea;
        tea.mCanvas = this;
        tea.mMouseEventArgs = e;
        tool->mouseButtonDown(tea);
    }
    invalidate();
}

void CanvasWindow::mouseButtonUp(const MouseEventArgs& e) {
    auto tool = RongtuliApp::getInstance()->getCurrentTool();
    if (tool != nullptr) {
        ToolEventArgs tea;
        tea.mCanvas = this;
        tea.mMouseEventArgs = e;
        tool->mouseButtonUp(tea);
    }
    invalidate();
}

void CanvasWindow::mouseMove(const MouseEventArgs& e) {
    auto tool = RongtuliApp::getInstance()->getCurrentTool();
    if (tool != nullptr) {
        ToolEventArgs tea;
        tea.mCanvas = this;
        tea.mMouseEventArgs = e;
        tool->mouseMove(tea);
    }
    invalidate();
}

void CanvasWindow::draw(Gdiplus::Graphics* g) {
    g->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    for (auto& obj : mObjectList) {
        obj->draw(g);
    }
    auto tool = RongtuliApp::getInstance()->getCurrentTool();
    if (tool != nullptr) {
        tool->draw(g);
    }
}


