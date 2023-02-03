#include "stdinclude.h"
#include "colorPanel.h"
#include "mainWindow.h"
#include "canvasWindow.h"

const float COLOR_SIZE_W = 26;
const float COLOR_SIZE_H = 26;

ColorPanel::ColorPanel(const HWND& parentWindowhandle)
    : Window(L"colorPanelClass", parentWindowhandle,
             10,
            10,
            60,
            720) {

}

ColorPanel::~ColorPanel() {

}

ColorButton* ColorPanel::addColor(const Gdiplus::Color& color) {
    std::shared_ptr<ColorButton> btn = std::make_shared<ColorButton>(color);
    mColorButtonList.push_back(btn);
    return btn.get();
}

void ColorPanel::mouseButtonDown(const MouseEventArgs& e) {
    const Gdiplus::PointF pt = {e.x, e.y};
    for (auto& btn : mColorButtonList) {
        btn->mSelected = false;
    }
    auto canvas = MainWindow::getInstance()->getCanvasWindow();
    for (auto& btn : mColorButtonList) {
        const auto& rect = btn->mRect;
        if (rect.Contains(pt)) {
            btn->mSelected = true;
            std::vector<std::shared_ptr<CanvasObject>> list;
            canvas->getSelectedObjects(list);
            for (auto& obj : list) {
                auto solidBrush = std::make_shared<Gdiplus::SolidBrush>(btn->getColor());
                obj->setBrush(solidBrush);
            }
            break;
        }
    }
    invalidate();
    canvas->invalidate();
}

void ColorPanel::mouseButtonUp(const MouseEventArgs& e) {

    invalidate();
}

void ColorPanel::mouseMove(const MouseEventArgs& e) {
    const Gdiplus::PointF pt = {e.x, e.y};
    for (auto& btn : mColorButtonList) {
        btn->mHighlight = false;
    }
    for (auto& btn : mColorButtonList) {
        const auto& rect = btn->mRect;
        if (rect.Contains(pt)) {
            btn->mHighlight = true;
            break;
        }
    }
    invalidate();
}

void ColorPanel::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen toolbarPen(Gdiplus::Color(56, 56, 56));
    Gdiplus::SolidBrush toolbarBrush(Gdiplus::Color(83, 83, 83));
    Gdiplus::SolidBrush toolbarTopBrush(Gdiplus::Color(66, 66, 66));
    const auto size = getSize();
    g->FillRectangle(&toolbarBrush, 0, 0, size.Width-1, size.Height-1);
    g->DrawRectangle(&toolbarPen, 0, 0, size.Width-1, size.Height-1);

    g->FillRectangle(&toolbarTopBrush, 0, 0, size.Width-1, 13);
    g->DrawRectangle(&toolbarPen, 0, 0, size.Width-1, 13);

    float toolBtnMargin = 5.0f;

    const float offsetStartX = toolBtnMargin;

    float offsetX = offsetStartX;
    float offsetY = 20;
    for (auto& btn : mColorButtonList) {
        //auto ff = Gdiplus::FontFamily(L"Arial");
        //Gdiplus::Font font(&ff, 10);
        //Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,100,100), Gdiplus::Color::Red, Gdiplus::Color::Yellow, Gdiplus::LinearGradientModeHorizontal);
        //g->DrawString(btn->getText().c_str(), -1, &font, Gdiplus::PointF(0, offsetY), &brush);


        Gdiplus::Pen pen(Gdiplus::Color(0, 102, 250), 2);
        Gdiplus::SolidBrush brush(btn->getColor());


        const auto x = offsetX;
        const auto y = offsetY;

        btn->mRect = {x, y, COLOR_SIZE_W, COLOR_SIZE_H};

        Gdiplus::RectF rect = btn->mRect;

        //if (btn->mSelected) {
            g->FillRectangle(&brush, rect);
        //}

        if (btn->mHighlight) {
            g->DrawRectangle(&pen, rect);
        }

        if ( (offsetX + COLOR_SIZE_W + toolBtnMargin + COLOR_SIZE_W) >= size.Width) {
            offsetY += COLOR_SIZE_H + toolBtnMargin;
            offsetX = offsetStartX;
        } else {
            offsetX += COLOR_SIZE_W + toolBtnMargin;
        }
    }
}

