//
//  LevelData.h -- A collection of Worksheets
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/DigitalQuizNS.h"
#include "DigitalQuizWorksheet.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_DIGITALQUIZ

class LevelData {
    typedef pair<string, int> LevelKey;
    typedef OneBasedVector<Worksheet> Level;
    map<LevelKey, Level> Levels;
    
public:
    size_t size() const;
    vector<int> levelIDsFor(const string& LanguageTag);
    Worksheet& randomSheetFor(const string& LanguageTag, int LevelID);

public:
    static LevelData defaultData();
    friend istream& operator>>(istream& IS, LevelData& LD);
};

END_NS_DIGITALQUIZ
