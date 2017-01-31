//
//  KeyFrame.h on Sep 6, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "KeyFrame.h"


BEGIN_NS_MOVINGINSECTS

KeyFrame::KeyFrame()
: BeginTime(0.f)
, EndTime(0.f)
{
}

KeyFrame::KeyFrame(float BeginTime, float EndTime)
: BeginTime(BeginTime)
, EndTime(EndTime)
{
}

float KeyFrame::beginTime() const {
    return BeginTime;
}

float KeyFrame::endTime() const {
    return EndTime;
}

float KeyFrame::interval() const {
    return EndTime - BeginTime;
}

float KeyFrame::interpolate(float Alpha) const {
    return ((1.f - Alpha) * BeginTime) + (Alpha * EndTime);
}


END_NS_MOVINGINSECTS