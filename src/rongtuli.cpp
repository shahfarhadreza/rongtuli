#include "stdinclude.h"
#include "tool.h"
#include "mainWindow.h"
#include "rongtuli.h"

GdiplusInitializer::GdiplusInitializer() {
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}
GdiplusInitializer::~GdiplusInitializer() {
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

RongtuliApp* RongtuliApp::mInstance = nullptr;

RongtuliApp::RongtuliApp(HINSTANCE hThisInstance)
    : mThisInstanceHandle(hThisInstance), mMainWindow(nullptr) {

    assert(mInstance == nullptr);
    mInstance = this;

    mGdiplus = std::make_shared<GdiplusInitializer>();

    mCurrentTool = std::make_shared<RectangleShapeTool>();

    mMainWindow = std::make_shared<MainWindow>("Rongtuli - v0.01");
}

RongtuliApp::~RongtuliApp() {

}

void RongtuliApp::setCurrentTool(const std::shared_ptr<Tool>& tool) {
    mCurrentTool = tool;
}

void RongtuliApp::addControl(HWND handle, Control* control) {
    mControls[handle] = control;
}

Control* RongtuliApp::findControl(HWND handle) {
    auto it = mControls.find(handle);
    if (it != mControls.end()) {
        return mControls[handle];
    }
    return nullptr;
}

void RongtuliApp::addWindow(HWND handle, Window* wnd) {
    mWindowMap[handle] = wnd;
}

Window* RongtuliApp::findWindow(HWND handle) {
    auto it = mWindowMap.find(handle);
    if (it != mWindowMap.end()) {
        return mWindowMap[handle];
    }
    return nullptr;
}

int RongtuliApp::run() {
    MSG messages;
    while (::GetMessage(&messages, nullptr, 0, 0)) {
        ::TranslateMessage(&messages);
        ::DispatchMessage(&messages);
    }
    return messages.wParam;
}



