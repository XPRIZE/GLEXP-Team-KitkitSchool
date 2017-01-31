//
//  TraceLineStyle.h -- Determine in what style trace lines are displayed
//  TodoSchool on Jul 3, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceLineStyle {
public:
    enum class Type {
        PassiveGuide,
        PassiveStroke,
        ActiveGuide,
        ActiveStroke
    };
    
public:
    Type TheType;
    float Thickness;
    Color4B Color;

public:
    TraceLineStyle(Type TheType, float Thickness, Color4B Color);
};

using TraceLineStyles = vector<TraceLineStyle>;

END_NS_TRACEFIELD
