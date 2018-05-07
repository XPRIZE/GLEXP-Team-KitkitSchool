//
//  AARect.h -- Axis aligned rectangle
//  TodoSchool on Aug 18, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): The goal is to make it compatible with cocos2d::Rect,
//   but we'll add more functionality on demand.


#pragma once

#include <cocos/math/Vec2.h>
#include <cocos/math/CCGeometry.h>
#include <string>



class AARect {
public:
    cocos2d::Point origin;
    cocos2d::Size size;

    std::string __repr__() const;

public:
    // NB(xenosoz, 2016): Please do not rely on AARect() is creating zero-rect.
    //   This behavior may be changed. Use AARect::zero() instead.
    AARect();

    AARect(float x, float y, float width, float height);
    AARect(const cocos2d::Point& origin, const cocos2d::Size& size);
    AARect(const AARect& rect);
    AARect(const cocos2d::Rect& rect);

    static AARect zero();

    // NB(xenosoz, 2016): True iff the rect is non-empty (area is non-zero).
    operator bool() const;
    operator cocos2d::Rect() const;
    
    AARect& operator=(const AARect& rect);
    AARect& operator=(const cocos2d::Rect& rect);
    
    bool operator==(const AARect& rect) const;
    bool operator==(const cocos2d::Rect& rect) const;
    
    bool contains(const cocos2d::Point& point) const;

    AARect intersection(const AARect& rect) const;
    AARect union_(const AARect& rect) const;

    AARect& intersectionUpdate(const AARect& rect);
    AARect& unionUpdate(const AARect& rect);
    
    float area() const;
    float minX() const;
    float minY() const;
    float maxX() const;
    float maxY() const;
    float midX() const;
    float midY() const;
};
    

