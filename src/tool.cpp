#include "stdinclude.h"
#include "canvasWindow.h"
#include "tool.h"

void RectangleShapeTool::mouseButtonUp(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    auto canvas = te.mCanvas;
    std::shared_ptr<RectangleObject> rect(new RectangleObject(e.x, e.y, 50, 50));
    canvas->insertObject(rect);
}

void EllipseShapeTool::mouseButtonUp(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    auto canvas = te.mCanvas;
    std::shared_ptr<EllipseObject> rect(new EllipseObject(e.x, e.y, 50, 50));
    canvas->insertObject(rect);
}

void LineShapeTool::mouseButtonUp(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    auto canvas = te.mCanvas;
    std::shared_ptr<LineObject> rect(new LineObject(e.x, e.y, 50, 50));
    canvas->insertObject(rect);
}

void TextTool::mouseButtonUp(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    auto canvas = te.mCanvas;
    std::shared_ptr<TextObject> rect(new TextObject(e.x, e.y, L"Farhad Reza"));
    canvas->insertObject(rect);
}

void SelectMoveTool::mouseButtonDown(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    auto canvas = te.mCanvas;
    auto list = canvas->getObjectList();
    for(auto it = list.rbegin(); it != list.rend();it++) {
        auto obj = *it;
        bool result = obj->intersect({e.x, e.y});
        if (result && obj->isSelected()) {
            mDragging = true;
            mDragStart = {e.x, e.y};
            mOffset = mDragStart - obj->getPosition();
            break;
        }
    }
}

void SelectMoveTool::mouseMove(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    const Gdiplus::PointF mousePt = {e.x, e.y};
    auto canvas = te.mCanvas;
    auto list = canvas->getObjectList();
    if (mDragging) {
        for(const auto& obj : list) {
            if (obj->isSelected()) {
                obj->setPosition(mousePt - mOffset);
                break;
            }
        }
    }
}

void SelectMoveTool::mouseButtonUp(const ToolEventArgs& te) {
    const auto& e = te.mMouseEventArgs;
    auto canvas = te.mCanvas;
    auto list = canvas->getObjectList();
    if (mDragging) {
        mDragging = false;
    } else {
        for(const auto& obj : list) {
            obj->setSelected(false);
        }
        for(auto it = list.rbegin(); it != list.rend();it++) {
            auto obj = *it;
            bool result = obj->intersect({e.x, e.y});
            if (result) {
                obj->setSelected(true);
                break;
            }
        }
    }
}

