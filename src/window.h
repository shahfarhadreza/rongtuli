#pragma once

class Window;
class CanvasWindow;

class MouseEventArgs {
public:
    float x, y;
};

class ToolEventArgs {
public:
    MouseEventArgs mMouseEventArgs;
    CanvasWindow* mCanvas;
};

class Window {
protected:
    HWND mWindowHandle;
public:
    HBITMAP mDoubleBufferBitmap;
    Window(const std::wstring& className,
               const HWND& parentWindowHandle,
               int x, int y, int width, int height);
    virtual ~Window();
    const HWND& getWindowHandle() const { return mWindowHandle; }
public:
    bool create();
    void invalidate();
    void setSize(const Gdiplus::Size& size);
    Gdiplus::Size getSize();
    void moveAndSize(int x, int y, int width, int height);
    virtual void mouseButtonDown(const MouseEventArgs& e) { }
    virtual void mouseButtonUp(const MouseEventArgs& e) { }
    virtual void mouseMove(const MouseEventArgs& e) { }
    virtual void draw(Gdiplus::Graphics* g) { }

    void _updateDoubleBuffer();
};

