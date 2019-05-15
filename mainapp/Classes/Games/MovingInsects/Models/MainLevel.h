//
//  MainLevel.h
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MovingInsectsNS.h"
#include "MainSheet.h"

BEGIN_NS_MOVINGINSECTS


class MainLevel {
    OneBasedVector<MainSheet> Sheets;

public:
    MainSheet& sheetByID(size_t SheetID);
    MainSheet& randomSheet(size_t *SheetID);
    
public:
    friend istream& operator>>(istream& IS, MainLevel& Level);
};

END_NS_MOVINGINSECTS
