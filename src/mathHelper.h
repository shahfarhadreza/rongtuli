#pragma once

inline void GetLine(double x1, double y1, double x2, double y2, double &a, double &b, double &c)
{
    a = y1 - y2;
    b = x2 - x1;
    c = x1 * y2 - x2 * y1;
}

inline double PointToLineDistance(double pct1X, double pct1Y, double pct2X, double pct2Y, double pct3X, double pct3Y)
{
    double a, b, c;
    GetLine(pct2X, pct2Y, pct3X, pct3Y, a, b, c);
    return std::abs(a * pct1X + b * pct1Y + c) / std::sqrt(a * a + b * b);
}

inline Gdiplus::RectF CalculateRectFromPoints(const std::vector<Gdiplus::PointF>& v) {
    auto xExtremes = std::minmax_element(v.begin(), v.end(),
                                         [](const Gdiplus::PointF& lhs, const Gdiplus::PointF& rhs) {
                                            return lhs.X < rhs.X;
                                         });

    auto yExtremes = std::minmax_element(v.begin(), v.end(),
                                         [](const Gdiplus::PointF& lhs, const Gdiplus::PointF& rhs) {
                                            return lhs.Y < rhs.Y;
                                         });

    Gdiplus::PointF upperLeft(xExtremes.first->X, yExtremes.first->Y);
    Gdiplus::PointF lowerRight(xExtremes.second->X, yExtremes.second->Y);
    return Gdiplus::RectF(upperLeft.X, upperLeft.Y, lowerRight.X - upperLeft.X, lowerRight.Y - upperLeft.Y);
}

