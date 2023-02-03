#include "stdinclude.h"
#include "propertiesPanel.h"
#include "mainWindow.h"
#include "canvasWindow.h"
#include "edit.h"

PropertiesPanel::PropertiesPanel(const HWND& parentWindowhandle)
    : Window(L"propertiesPanelClass", parentWindowhandle,
             10,
            10,
            60,
            720) {

    mEditText = std::make_shared<Edit>(getWindowHandle(), L"Farhad Reza", 10, 25, 150, 26);
    mEditText->onEnter([this]() {
        auto canvas = MainWindow::getInstance()->getCanvasWindow();
        std::vector<std::shared_ptr<CanvasObject>> list;
        canvas->getSelectedObjects(list);
        if (list.size() == 1) {
            auto obj = list[0]->asTextObject();
            if (obj) {
                obj->setText(mEditText->getText());
            }
        }

        canvas->invalidate();
    });
}

PropertiesPanel::~PropertiesPanel() {

}

void PropertiesPanel::mouseButtonDown(const MouseEventArgs& e) {

    invalidate();
}

void PropertiesPanel::mouseButtonUp(const MouseEventArgs& e) {

    invalidate();
}

void PropertiesPanel::mouseMove(const MouseEventArgs& e) {
    //const Gdiplus::PointF pt = {e.x, e.y};
    invalidate();
}

void PropertiesPanel::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen toolbarPen(Gdiplus::Color(56, 56, 56));
    Gdiplus::SolidBrush toolbarBrush(Gdiplus::Color(83, 83, 83));
    Gdiplus::SolidBrush toolbarTopBrush(Gdiplus::Color(66, 66, 66));
    const auto size = getSize();
    g->FillRectangle(&toolbarBrush, 0, 0, size.Width-1, size.Height-1);
    g->DrawRectangle(&toolbarPen, 0, 0, size.Width-1, size.Height-1);

    g->FillRectangle(&toolbarTopBrush, 0, 0, size.Width-1, 13);
    g->DrawRectangle(&toolbarPen, 0, 0, size.Width-1, 13);
}

