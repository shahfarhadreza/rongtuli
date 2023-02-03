#pragma once

#include "control.h"
#include "mainWindow.h"

class Window;
class Tool;

class GdiplusInitializer {
private:
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
public:
    GdiplusInitializer();
    ~GdiplusInitializer();
};

class RongtuliApp {
private:
    static RongtuliApp* mInstance;
protected:
    HINSTANCE mThisInstanceHandle;
    std::shared_ptr<GdiplusInitializer> mGdiplus;
    std::map<HWND, Control*> mControls;
    std::map<HWND, Window*> mWindowMap;
    std::shared_ptr<Tool> mCurrentTool;
    std::shared_ptr<MainWindow> mMainWindow;
public:
    RongtuliApp(HINSTANCE hThisInstance);
    ~RongtuliApp();
    static RongtuliApp* getInstance() { return mInstance; }
    const HINSTANCE& getInstanceHandle() const { return mThisInstanceHandle; }
    std::shared_ptr<MainWindow> getMainWindow() { return mMainWindow; }
    void setCurrentTool(const std::shared_ptr<Tool>& tool);
    std::shared_ptr<Tool> getCurrentTool() { return mCurrentTool; }
    void addControl(HWND handle, Control* control);
    Control* findControl(HWND handle);
    void addWindow(HWND handle, Window* wnd);
    Window* findWindow(HWND handle);
    int run();
};

