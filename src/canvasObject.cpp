#include "stdinclude.h"
#include "canvasObject.h"
#include "mathHelper.h"

void DrawTransformGrips(Gdiplus::Graphics* g, const Gdiplus::PointF& topleft, const Gdiplus::PointF& bottomright) {
    Gdiplus::Pen bluePen(Gdiplus::Color(0, 0, 255));
    Gdiplus::Pen dashPen(Gdiplus::Color(0, 0, 255));
    Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255));
    const auto gripSize = 6.0f;
    const auto gripSizeHalf = gripSize / 2.0f;
    const auto x1 = topleft.X;
    const auto y1 = topleft.Y;
    const auto x2 = bottomright.X;
    const auto y2 = bottomright.Y;

    dashPen.SetDashStyle(Gdiplus::DashStyleDash);
    dashPen.SetDashOffset(10);

    g->DrawRectangle(&dashPen, x1, y1, x2 - x1, y2 - y1);

    g->FillRectangle(&whiteBrush,
            x1 - gripSizeHalf, y1 - gripSizeHalf,
            gripSize, gripSize);
    g->FillRectangle(&whiteBrush,
            x1 - gripSizeHalf, y2 - gripSizeHalf,
            gripSize, gripSize);
    g->FillRectangle(&whiteBrush,
            x2 - gripSizeHalf, y1 - gripSizeHalf,
            gripSize, gripSize);
    g->FillRectangle(&whiteBrush,
            x2 - gripSizeHalf, y2 - gripSizeHalf,
            gripSize, gripSize);

    g->DrawRectangle(&bluePen,
            x1 - gripSizeHalf, y1 - gripSizeHalf,
            gripSize, gripSize);
    g->DrawRectangle(&bluePen,
            x1 - gripSizeHalf, y2 - gripSizeHalf,
            gripSize, gripSize);
    g->DrawRectangle(&bluePen,
            x2 - gripSizeHalf, y1 - gripSizeHalf,
            gripSize, gripSize);
    g->DrawRectangle(&bluePen,
            x2 - gripSizeHalf, y2 - gripSizeHalf,
            gripSize, gripSize);
}

CanvasObject::CanvasObject()
    : mX(0), mY(0), mSelected(false),
    mPen(nullptr), mBrush(nullptr) {

    mPen = std::make_shared<Gdiplus::Pen>(Gdiplus::Color(0, 0, 0));
}

void CanvasObject::setBrush(const std::shared_ptr<Gdiplus::Brush>& brush) {
    mBrush = brush;
}

bool RectangleObject::intersect(const Gdiplus::PointF& pt) const {
    if (pt.X >= mX && pt.X <= mX + mWidth) {
        if (pt.Y >= mY && pt.Y <= mY + mHeight) {
            return true;
        }
    }
    return false;
}

void RectangleObject::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen redPen(Gdiplus::Color(255, 0, 0));
    if (mBrush) {
        g->FillRectangle(mBrush.get(), mX, mY, mWidth, mHeight);
    }
    if (this->isSelected()) {
        g->DrawRectangle(&redPen, mX, mY, mWidth, mHeight);
        DrawTransformGrips(g, {mX, mY}, {mX + mWidth, mY + mHeight});
    } else {
        g->DrawRectangle(mPen.get(), mX, mY,  mWidth, mHeight);
    }
}

bool EllipseObject::intersect(const Gdiplus::PointF& pt) const {
    if (pt.X >= mX && pt.X <= mX + mWidth) {
        if (pt.Y >= mY && pt.Y <= mY + mHeight) {
            return true;
        }
    }
    return false;
}

void EllipseObject::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen redPen(Gdiplus::Color(255, 0, 0));
    if (mBrush) {
        g->FillEllipse(mBrush.get(), mX, mY, mWidth, mHeight);
    }
    if (this->isSelected()) {
        g->DrawEllipse(&redPen, mX, mY, mWidth, mHeight);
        DrawTransformGrips(g, {mX, mY}, {mX + mWidth, mY + mHeight});
    } else {
        g->DrawEllipse(mPen.get(), mX, mY,  mWidth, mHeight);
    }
}

bool LineObject::intersect(const Gdiplus::PointF& pt) const {

    double d = PointToLineDistance(pt.X, pt.Y, mX, mY, mX2, mY2);
    if (d < 3.5) {
        return true;
    }
    return false;
}

void LineObject::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen redPen(Gdiplus::Color(255, 0, 0));

    mX2 = mX + mWidth;
    mY2 = mY + mHeight;

    std::vector<Gdiplus::PointF> v;
    v.push_back({mX, mY});
    v.push_back({mX2, mY2});

    const auto rect = CalculateRectFromPoints(v);

    mWidth = rect.Width;
    mHeight = rect.Height;

    if (this->isSelected()) {
        g->DrawLine(&redPen, mX, mY,  mX2, mY2);
        DrawTransformGrips(g, {mX, mY}, {mX + mWidth, mY + mHeight});
    } else {
        g->DrawLine(mPen.get(), mX, mY,  mX2, mY2);
    }
}

bool TextObject::intersect(const Gdiplus::PointF& pt) const {
    if (pt.X >= mX && pt.X <= mX + mWidth) {
        if (pt.Y >= mY && pt.Y <= mY + mHeight) {
            return true;
        }
    }
    return false;
}

void TextObject::draw(Gdiplus::Graphics* g) {
    Gdiplus::Pen redPen(Gdiplus::Color(255, 0, 0));

    auto ff = Gdiplus::FontFamily(L"Arial");
    Gdiplus::Font font(&ff, 20);

    Gdiplus::RectF layoutRect(mX, mY, 10000, 10000);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentNear);
    Gdiplus::RectF boundRect;

    g->MeasureString(mText.c_str(), mText.length(), &font, layoutRect, &format, &boundRect);

    mWidth = boundRect.Width;
    mHeight = boundRect.Height;

    if (mBrush) {
        g->DrawString(mText.c_str(), -1, &font, Gdiplus::PointF(mX, mY), mBrush.get());
    } else {
        Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,100,100), Gdiplus::Color::Red, Gdiplus::Color::Yellow, Gdiplus::LinearGradientModeHorizontal);
        g->DrawString(mText.c_str(), -1, &font, Gdiplus::PointF(mX, mY), &brush);
    }

    if (this->isSelected()) {
        DrawTransformGrips(g, {mX, mY}, {mX + mWidth, mY + mHeight});
    } else {
        //g->DrawRectangle(mPen.get(), mX, mY,  mWidth, mHeight);
    }
}

