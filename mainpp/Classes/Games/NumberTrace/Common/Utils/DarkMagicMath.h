//
//  DarkMagicMath.h on Apr 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//  which is again from Todomath iOS (Locomotive Labs)
//

#ifndef TODOSCHOOL_UTILS_DARKMAGICMATH_H
#define TODOSCHOOL_UTILS_DARKMAGICMATH_H

#include <cocos/math/Vec2.h>
#include <cocos/math/CCGeometry.h>

namespace todoschool {

//
// NB(xenosoz, 2016): Spline is a curve segment with some control points.
//   I hope you heared of the Bezier curve as well, the curve itself doesn't intersect control points.
//   Here comes the strong points of Spline:
//     it contains control points, roughly speaking, making it easier to manipulate.
//   We can implement Catmull-Rom spline in terms of Cubic Bezier curve.
//   That's what we're doing here right now.
//
class DarkMagicMath {
public:
    typedef cocos2d::Point Point;
    typedef cocos2d::Size Size;
    typedef cocos2d::Vec2 Vec2;

    float DistanceBetweenPoints(Point a, Point b);
    Point MidpointWithAlpha(Point a, Point b, float alpha);
    float DistanceFromOrigin(Point point);
    float DistanceFromOrigin(Size size);
    Point Normalized(Point point);
    Size Normalized(Size size);
    float NormalizedAngle(float angle, float center);
    float Atan2OfPoint(Point point);

    float CubicBezier(float a, float b, float c, float d, float t);
    float CubicBezierTangential(float a, float b, float c, float d, float t);
    float CatmullRom(float a, float b, float c, float d, float t);
    float CatmullRomTangential(float a, float b, float c, float d, float t);

    Point CubicBezier(Point a, Point b, Point c, Point d, float t);
    Point CatmullRom(Point a, Point b, Point c, Point d, float t);

    Vec2 CubicBezierTangential(Point a, Point b, Point c, Point d, float t);
    Vec2 CubicBezierNormal(Point a, Point b, Point c, Point d, float t);
    Vec2 CatmullRomTangential(Point a, Point b, Point c, Point d, float t);
    Vec2 CatmullRomNormal(Point a, Point b, Point c, Point d, float t);
};

} // namespace todoschool
#endif // !defined(TODOSCHOOL_UTILS_DARKMAGICMATH_H)