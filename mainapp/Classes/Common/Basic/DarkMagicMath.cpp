//
//  DarkMagicMath.cpp on Apr 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//  which is again from Todomath iOS (Locomotive Labs)
//

#include "DarkMagicMath.h"

using namespace todoschool;
using namespace cocos2d;

// from Point+.m (Todomath iOS)

float DarkMagicMath::DistanceBetweenPoints(Point a, Point b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dist = sqrtf(dx*dx + dy*dy);
    return dist;
}

Point DarkMagicMath::MidpointWithAlpha(Point a, Point b, float alpha) {
    Point point {a.x*alpha + b.x*(1.f-alpha), a.y*alpha + b.y*(1.f-alpha)};
    return point;
}

float DarkMagicMath::DistanceFromOrigin(Point point) {
    float dist = sqrtf(point.x*point.x + point.y*point.y);
    return dist;
}

float DarkMagicMath::DistanceFromOrigin(Size size) {
    float dist = sqrtf(size.width*size.width + size.height*size.height);
    return dist;
}

Point DarkMagicMath::Normalized(Point point) {
    float dist = DistanceFromOrigin(point);
    if (dist < 1e-9) {
        return Point{0.f, 0.f};
    }
    return Point{point.x/dist, point.y/dist};
}

Size DarkMagicMath::Normalized(Size size) {
    float dist = DistanceFromOrigin(size);
    if (dist < 1e-9) {
        return Size{0.f, 0.f};
    }
    return Size{size.width/dist, size.height/dist};
}

float DarkMagicMath::NormalizedAngle(float angle, float center) {
    // XXX: We definitely need better implementation here.
    
    while (angle <= center - M_PI)
        angle += 2 * M_PI;

    while (angle >= center + M_PI)
        angle -= 2 * M_PI;
    
    return angle;
}

float DarkMagicMath::Atan2OfPoint(Point point) {
    return atan2(point.y, point.x);
}


float DarkMagicMath::CubicBezier(float a, float b, float c, float d, float t) {
    // Make a path segment from a to d.
    float ab = (1-t)*a + t*b;
    float bc = (1-t)*b + t*c;
    float cd = (1-t)*c + t*d;
    float abc = (1-t)*ab + t*bc;
    float bcd = (1-t)*bc + t*cd;
    float abcd = (1-t)*abc + t*bcd;
    return abcd;
}

float DarkMagicMath::CubicBezierTangential(float a, float b, float c, float d, float t) {
    float ab = (1-t)*a + t*b;
    float bc = (1-t)*b + t*c;
    float cd = (1-t)*c + t*d;
    float abc = (1-t)*ab + t*bc;
    float bcd = (1-t)*bc + t*cd;

    // Derivatives.
    float ab_ = -a + b;
    float bc_ = -b + c;
    float cd_ = -c + d;
    float abc_ = -ab + (1-t)*ab_ + bc + t*bc_;
    float bcd_ = -bc + (1-t)*bc_ + cd + t*cd_;
    float abcd_ = -abc + (1-t)*abc_ + bcd + t*bcd_;

    return abcd_;
}

float DarkMagicMath::CatmullRom(float a, float b, float c, float d, float t) {
    // Make a path segment from b to c.
    float bPrime = b+(c-a)/6.f;
    float cPrime = c+(b-d)/6.f;
    return CubicBezier(b, bPrime, cPrime, c, t);
}

float DarkMagicMath::CatmullRomTangential(float a, float b, float c, float d, float t) {
    float bPrime = b+(c-a)/6.f;
    float cPrime = c+(b-d)/6.f;
    return CubicBezierTangential(b, bPrime, cPrime, c, t);
}


Point DarkMagicMath::CubicBezier(Point a, Point b, Point c, Point d, float t) {
    float x = CubicBezier(a.x, b.x, c.x, d.x, t);
    float y = CubicBezier(a.y, b.y, c.y, d.y, t);
    return Point{x, y};
}

Point DarkMagicMath::CatmullRom(Point a, Point b, Point c, Point d, float t) {
    float x = CatmullRom(a.x, b.x, c.x, d.x, t);
    float y = CatmullRom(a.y, b.y, c.y, d.y, t);
    return Point{x, y};
}


Vec2 DarkMagicMath::CubicBezierTangential(Point a, Point b, Point c, Point d, float t) {
    float dx = CubicBezierTangential(a.x, b.x, c.x, d.x, t);
    float dy = CubicBezierTangential(a.y, b.y, c.y, d.y, t);
    return Vec2{dx, dy};
}

Vec2 DarkMagicMath::CubicBezierNormal(Point a, Point b, Point c, Point d, float t) {
    float dx = CubicBezierTangential(a.x, b.x, c.x, d.x, t);
    float dy = CubicBezierTangential(a.y, b.y, c.y, d.y, t);
    return Vec2{dy, -dx};
}

Vec2 DarkMagicMath::CatmullRomTangential(Point a, Point b, Point c, Point d, float t) {
    // NB(xenosoz, 2016): It should be CatmullRom*. There was a bug in my old code. (2013?)
    float dx = CatmullRomTangential(a.x, b.x, c.x, d.x, t);
    float dy = CatmullRomTangential(a.y, b.y, c.y, d.y, t);
    return Vec2{dx, dy};
}

Vec2 DarkMagicMath::CatmullRomNormal(Point a, Point b, Point c, Point d, float t) {
    // NB(xenosoz, 2016): It should be CatmullRom*. There was a bug in my old code. (2013?)
    float dx = CatmullRomTangential(a.x, b.x, c.x, d.x, t);
    float dy = CatmullRomTangential(a.y, b.y, c.y, d.y, t);
    return Vec2{dy, -dx};
}
