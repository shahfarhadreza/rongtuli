#pragma once

class TextObject;

class CanvasObject {
protected:
    float mX, mY;
    bool mSelected;
    std::shared_ptr<Gdiplus::Pen> mPen;
    std::shared_ptr<Gdiplus::Brush> mBrush;
public:
    CanvasObject();
    virtual std::string toString() {
        return "this a simple canvas object";
    }
    virtual void draw(Gdiplus::Graphics* g) = 0;
    virtual bool intersect(const Gdiplus::PointF& pt) const { return false; }
    void setPosition(const Gdiplus::PointF& pt) { mX = pt.X; mY = pt.Y; }
    const Gdiplus::PointF getPosition() const { return {mX, mY}; }
    void setSelected(bool selected) { mSelected = selected; }
    bool isSelected() const { return mSelected; }
    void setBrush(const std::shared_ptr<Gdiplus::Brush>& brush);

    virtual TextObject* asTextObject() { return nullptr; }
};

class RectangleObject : public CanvasObject {
protected:
    float mWidth, mHeight;
public:
    RectangleObject(float x, float y, float width, float height) {
        mX = x;
        mY = y;
        mWidth = width;
        mHeight = height;
    }
    virtual std::string toString() {
        return "this a simple rectangle object";
    }
    virtual bool intersect(const Gdiplus::PointF& pt) const;
    virtual void draw(Gdiplus::Graphics* g);
};

class EllipseObject : public CanvasObject {
protected:
    float mWidth, mHeight;
public:
    EllipseObject(float x, float y, float width, float height) {
        mX = x;
        mY = y;
        mWidth = width;
        mHeight = height;
    }
    virtual std::string toString() {
        return "this a simple ellipse object";
    }
    virtual bool intersect(const Gdiplus::PointF& pt) const;
    virtual void draw(Gdiplus::Graphics* g);
};

class LineObject : public CanvasObject {
protected:
    float mWidth, mHeight;
    float mX2, mY2;
public:
    LineObject(float x, float y, float w, float h) {
        mX = x;
        mY = y;
        mWidth = w;
        mHeight = h;

        mX2 = mX + mWidth;
        mY2 = mY + mHeight;
    }
    virtual std::string toString() {
        return "this a simple line object";
    }
    virtual bool intersect(const Gdiplus::PointF& pt) const;
    virtual void draw(Gdiplus::Graphics* g);
};

class TextObject : public CanvasObject {
protected:
    std::wstring mText;
    float mWidth, mHeight;
public:
    TextObject(float x, float y, const std::wstring& text) {
        mX = x;
        mY = y;
        mWidth = 0;
        mHeight = 0;
        mText = text;
    }
    virtual std::string toString() {
        return "this a simple text object";
    }
    virtual bool intersect(const Gdiplus::PointF& pt) const;
    virtual void draw(Gdiplus::Graphics* g);

    void setText(const std::wstring& text) {
        mText = text;
    }

    virtual TextObject* asTextObject() { return this; }
};



