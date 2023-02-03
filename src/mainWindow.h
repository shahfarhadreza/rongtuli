#pragma once

class ToolBar;
class ColorPanel;
class CanvasWindow;
class PropertiesPanel;
class Button;

class MainWindow {
private:
    static MainWindow* mInstance;
protected:
    HWND mWindowHandle;
    bool mInitialized;
    std::shared_ptr<ToolBar> mToolBar;
    std::shared_ptr<CanvasWindow> mCanvasWindow;
    std::shared_ptr<ColorPanel> mColorPanel;
    std::shared_ptr<PropertiesPanel> mPropertiesPanel;
public:
    MainWindow(const std::string& title);
    ~MainWindow();
    void reLayout(const Gdiplus::Size& size);
    static MainWindow* getInstance() { return mInstance; }
    const HWND& getWindowHandle() const { return mWindowHandle; }
    std::shared_ptr<CanvasWindow> getCanvasWindow() { return mCanvasWindow; }
};

