//
//  MainSheet.h -- Main worksheet
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"
#include "MainWork.h"

BEGIN_NS_MOVINGINSECTS


class MainSheet {
    OneBasedVector<MainWork> Works;
    
public:
    size_t size() const;
    size_t beginWorkID() const;
    size_t endWorkID() const;
    MainWork& workByID(size_t WorkID);
    
public:
    friend istream& operator>>(istream& IS, MainSheet& Sheet);
};

END_NS_MOVINGINSECTS