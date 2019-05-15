//
//  TracePoint.h -- Smallest unit for trace fonts with single point position and velocity
//  TodoSchool on Apr 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "TracePoint.h"
#include <cocos/math/Vec2.h>


BEGIN_NS_TRACEFIELD

TracePoint::TracePoint() {}

TracePoint::TracePoint(cocos2d::Point P, cocos2d::Vec2 V)
    : Position(P), Velocity(V) {}

Rect TracePoint::boundingBox() const {
    Rect R;
    R.origin = Position;
    R.size = Size::ZERO;

    return R;
}

END_NS_TRACEFIELD