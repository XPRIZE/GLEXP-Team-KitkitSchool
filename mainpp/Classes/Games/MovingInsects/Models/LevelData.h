//
//  LevelData.h on Sep 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"
#include "MainLevel.h"

BEGIN_NS_MOVINGINSECTS


class LevelData {
    OneBasedVector<MainLevel> Levels;
    
public:
    size_t size() const;
    size_t beginLevelID() const;
    size_t endLevelID() const;
    MainLevel& levelByID(size_t LevelID);

public:
    static LevelData defaultData();
    friend istream& operator>>(istream& IS, LevelData& Data);
};

END_NS_MOVINGINSECTS