//
//  LevelData.h -- Level data for FindTheMatch
//  TodoSchool on Oct 6, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Worksheet.h"
#include "../Utils/MainNS.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_FINDTHEMATCH

class LevelData {
    typedef OneBasedVector<Worksheet> Level;
    OneBasedVector<Level> Levels;

public:
    size_t size() const;
    vector<int> levelIDs() const;
    Worksheet& randomSheetFor(int LevelID);
    
public:
    static LevelData defaultData();
    friend istream& operator>>(istream& IS, LevelData& Data);
};

END_NS_FINDTHEMATCH