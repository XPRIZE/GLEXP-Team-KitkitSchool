//
//  AARect.cpp -- Axis aligned rectangle
//  TodoSchool on Aug 18, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "AARect.h"
#include "../Repr/AllRepr.h"
#include <cmath>
#include <sstream>
#include <algorithm>


namespace todoschool {

std::string AARect::__repr__() const {
    std::stringstream SS;
    SS << "todoschool::AARect(";
    SS << "origin: " << repr(origin) << ", ";
    SS << "size: " << repr(size) << ")";
    return SS.str();
}

AARect::AARect() : AARect(0.f, 0.f, 0.f, 0.f) {}

AARect::AARect(float x, float y, float width, float height)
    : origin(x, y), size(width, height) {}

AARect::AARect(const cocos2d::Point& origin, const cocos2d::Size& size)
    : origin(origin), size(size) {}

AARect::AARect(const AARect& rect) : origin(rect.origin), size(rect.size) {}
AARect::AARect(const cocos2d::Rect& rect) : origin(rect.origin), size(rect.size) {}

AARect AARect::zero() {
    return AARect(0.f, 0.f, 0.f, 0.f);
}

AARect::operator bool() const {
    return (size.width != 0.f && size.height != 0.f);
}

AARect::operator cocos2d::Rect() const {
    return cocos2d::Rect(origin, size);
}
    
AARect& AARect::operator=(const AARect& rect) {
    origin = rect.origin;
    size = rect.size;
    return *this;
}

AARect& AARect::operator=(const cocos2d::Rect& rect) {
    origin = rect.origin;
    size = rect.size;
    return *this;
}

bool AARect::operator==(const AARect& rect) const {
    return (origin.equals(rect.origin) &&
            size.equals(rect.size));
}
bool AARect::operator==(const cocos2d::Rect& rect) const {
    return (origin.equals(rect.origin) &&
            size.equals(rect.size));
}

bool AARect::contains(const cocos2d::Point& point) const {
    return ((minX() <= point.x && point.x <= maxX()) &&
            (minY() <= point.y && point.y <= maxY()));
}

AARect AARect::intersection(const AARect& rect) const {
    auto& lhs = *this;
    auto& rhs = rect;
    
    float minX = std::max(lhs.minX(), rhs.minX());
    float minY = std::max(lhs.minY(), rhs.minY());
    float maxX = std::min(lhs.maxX(), rhs.maxX());
    float maxY = std::min(lhs.maxY(), rhs.maxY());
    
    if (minX >= maxX || minY >= maxY) {
        // NB(xenosoz, 2016): It doesn't intersect (area is zero).
        return zero();
    }

    return AARect(minX, minY, (maxX - minX), (maxY - minY));
}

AARect AARect::union_(const AARect& rect) const {
    auto& lhs = *this;
    auto& rhs = rect;

    float minX = std::min(lhs.minX(), rhs.minX());
    float minY = std::min(lhs.minY(), rhs.minY());
    float maxX = std::max(lhs.maxX(), rhs.maxX());
    float maxY = std::max(lhs.maxY(), rhs.maxY());
    
    return AARect(minX, minY, (maxX - minX), (maxY - minY));
}

AARect& AARect::intersectionUpdate(const AARect& rect) {
    AARect it = intersection(rect);
    std::swap(*this, it);
    
    return *this;
}

AARect& AARect::unionUpdate(const AARect& rect) {
    AARect it = union_(rect);
    std::swap(*this, it);

    return *this;
}

float AARect::area() const { return std::fabs(size.width * size.height); }
float AARect::minX() const { return origin.x + std::min(size.width, 0.f); }
float AARect::minY() const { return origin.y + std::min(size.height, 0.f); }
float AARect::maxX() const { return origin.x + std::max(0.f, size.width); }
float AARect::maxY() const { return origin.y + std::max(0.f, size.height); }
float AARect::midX() const { return origin.x + size.width / 2.f; }
float AARect::midY() const { return origin.y + size.height / 2.f; }

}  // namespace todoschool