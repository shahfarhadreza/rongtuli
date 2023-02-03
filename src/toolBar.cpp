#include "stdinclude.h"
#include "toolBar.h"

const float TOOL_SIZE_W = 36;
const float TOOL_SIZE_H = 29;

void DrawRoundRectangle(Gdiplus::Graphics* g, Gdiplus::Pen *p, Gdiplus::RectF& rect, float radius)
{
	if (g==NULL) return;
	Gdiplus::GraphicsPath path;

	path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
	path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2,
		radius * 2, 0, 90);
	path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
	path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
	path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();
	g->DrawPath(p, &path);
}

void FillRoundRectangle(Gdiplus::Graphics* g, Gdiplus::Brush *p, Gdiplus::RectF& rect, float radius)
{
	if (g == NULL) return;
	Gdiplus::GraphicsPath path;

	path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
	path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
	path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
	path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
	path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	g->FillPath(p, &path);
}

ToolBar::ToolBar(const HWND& parentWindowhandle)
    : Window(L"toolBarClass", parentWindowhandle,
             10,
            10,
            60,
            720) {

}

ToolBar::~ToolBar() {

}

void ToolBar::setDefaultTool(ToolButton* tool) {
    for (auto& btn : mToolButtonList) {
        btn->mSelected = false;
    }
    tool->mSelected = true;
    invalidate();
}

ToolButton* ToolBar::addButton(const std::wstring& text, const std::wstring& icon, const std::function<void()>& onClick) {
    std::shared_ptr<ToolButton> btn = std::make_shared<ToolButton>(text, icon, onClick);
    mToolButtonList.push_back(btn);
    return btn.get();
}

void ToolBar::mouseButtonDown(const MouseEventArgs& e) {
    const Gdiplus::PointF pt = {e.x, e.y};
    for (auto& btn : mToolButtonList) {
        btn->mSelected = false;
    }
    for (auto& btn : mToolButtonList) {
        const auto& rect = btn->mRect;
        if (rect.Contains(pt)) {
            btn->mSelected = true;
            if (btn->mOnClick) {
                btn->mOnClick();
            }
            break;
        }
    }
    invalidate();
}

void ToolBar::mouseButtonUp(const MouseEventArgs& e) {

    invalidate();
}

void ToolBar::mouseMove(const MouseEventArgs& e) {
    const Gdiplus::PointF pt = {e.x, e.y};
    for (auto& btn : mToolButtonList) {
        btn->mHighlight = false;
    }
    for (auto& btn : mToolButtonList) {
        const auto& rect = btn->mRect;
        if (rect.Contains(pt)) {
            btn->mHighlight = true;
            break;
        }
    }
    invalidate();
}

void ToolBar::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen toolbarPen(Gdiplus::Color(56, 56, 56));
    Gdiplus::SolidBrush toolbarBrush(Gdiplus::Color(83, 83, 83));
    Gdiplus::SolidBrush toolbarTopBrush(Gdiplus::Color(66, 66, 66));
    const auto size = getSize();
    g->FillRectangle(&toolbarBrush, 0, 0, size.Width-1, size.Height-1);
    g->DrawRectangle(&toolbarPen, 0, 0, size.Width-1, size.Height-1);

    g->FillRectangle(&toolbarTopBrush, 0, 0, size.Width-1, 13);
    g->DrawRectangle(&toolbarPen, 0, 0, size.Width-1, 13);

    float offsetX = 3;
    float offsetY = 20;
    float toolBtnMargin = 2.0f;
    for (auto& btn : mToolButtonList) {


        auto image = btn->getImage();

        Gdiplus::Pen pen(Gdiplus::Color(102, 102, 102));
        Gdiplus::SolidBrush brush(Gdiplus::Color(69, 69, 69));
        Gdiplus::SolidBrush brushS(Gdiplus::Color(56, 56, 56));


        const auto x = offsetX;
        const auto y = offsetY;

        btn->mRect = {x, y, TOOL_SIZE_W, TOOL_SIZE_H};

        Gdiplus::RectF rect = btn->mRect;

        if (btn->mHighlight) {
            FillRoundRectangle(g, &brush, rect, 5);
            DrawRoundRectangle(g, &pen, rect, 5);
        }

        if (btn->mSelected) {
            FillRoundRectangle(g, &brushS, rect, 5);
            DrawRoundRectangle(g, &pen, rect, 5);
        }

        Gdiplus::RectF rectImg = {x + (TOOL_SIZE_W/2) - (TOOL_SIZE_H/2), y, TOOL_SIZE_H, TOOL_SIZE_H};

        rectImg.Inflate(-4, -4);

        g->DrawImage(image, rectImg);

        offsetY += TOOL_SIZE_H + toolBtnMargin;
    }
}

