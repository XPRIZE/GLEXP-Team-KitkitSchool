//
//  KeyFrame.h on Sep 6, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_MOVINGINSECTS

class KeyFrame {
    // NB(xenosoz, 2016): Those are in second(s)
    float BeginTime;
    float EndTime;

public:
    KeyFrame();
    KeyFrame(float BeginTime, float EndTime);

    float beginTime() const;
    float endTime() const;

    float interval() const;
    float interpolate(float Alpha) const;
};

END_NS_MOVINGINSECTS