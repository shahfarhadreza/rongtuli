#pragma once

class ToolEventArgs;
class CanvasWindow;

class Tool {
protected:
public:
    Tool() {

    }
    virtual void mouseButtonDown(const ToolEventArgs& e) {}
    virtual void mouseButtonUp(const ToolEventArgs& e) {}
    virtual void mouseMove(const ToolEventArgs& e) {}
    virtual void draw(Gdiplus::Graphics* g) {}
};

class RectangleShapeTool : public Tool {
public:
    RectangleShapeTool() : Tool() {
    }

    virtual void mouseButtonUp(const ToolEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g) {}
};

class EllipseShapeTool : public Tool {
public:
    EllipseShapeTool() : Tool() {
    }

    virtual void mouseButtonUp(const ToolEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g) {}
};

class LineShapeTool : public Tool {
public:
    LineShapeTool() : Tool() {
    }

    virtual void mouseButtonUp(const ToolEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g) {}
};

class SelectMoveTool : public Tool {
protected:
    bool mDragging;
    Gdiplus::PointF mDragStart;
    Gdiplus::PointF mOffset;
public:
    SelectMoveTool() : mDragging(false) {

    }
    virtual void mouseButtonDown(const ToolEventArgs& e);
    virtual void mouseButtonUp(const ToolEventArgs& e);
    virtual void mouseMove(const ToolEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g) {}
};

class TextTool : public Tool {
public:
    TextTool() : Tool() {
    }

    virtual void mouseButtonUp(const ToolEventArgs& e);
    virtual void draw(Gdiplus::Graphics* g) {}
};


