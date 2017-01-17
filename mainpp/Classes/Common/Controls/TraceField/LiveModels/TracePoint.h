//
//  TracePoint.h -- Smallest unit for trace fonts with single point position and velocity
//  TodoSchool on Apr 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TracePoint {
public:
    Point Position;
    Vec2 Velocity;
    
public:
    TracePoint();
    TracePoint(Point P, Vec2 V);
    
    // NB(xenosoz, 2016): Single-point bounding box with origin(Point), size(ZERO).
    Rect boundingBox() const;
};

END_NS_TRACEFIELD