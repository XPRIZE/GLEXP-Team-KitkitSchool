//
//  TraceLineStyle.cpp -- Determine in what style trace lines are displayed
//  TodoSchool on Jul 3, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "TraceLineStyle.h"


BEGIN_NS_TRACEFIELD

TraceLineStyle::TraceLineStyle(Type TheType, float Thickness, Color4B Color)
    : TheType(TheType)
    , Thickness(Thickness)
    , Color(Color)
{
}

END_NS_TRACEFIELD