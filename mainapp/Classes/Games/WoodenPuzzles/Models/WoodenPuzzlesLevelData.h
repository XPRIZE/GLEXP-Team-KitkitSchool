//
//  LevelData.h -- A collection of Worksheets
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/WoodenPuzzleNS.h"
#include "WoodenPuzzlesWorksheet.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_WOODENPUZZLES

class LevelData {
    typedef pair<string, size_t> LevelKey;
    typedef OneBasedVector<Worksheet> Level;
    map<LevelKey, Level> Levels;
    
public:
    vector<size_t> levelIDsFor(const string& LanguageTag);
    Worksheet& randomSheetFor(const string& LanguageTag, size_t LevelID, int *WorkSheetID);
    
public:
    static LevelData alphabetPuzzleData();
    static LevelData numberPuzzleData();
    friend istream& operator>>(istream& IS, LevelData& LD);
};

END_NS_WOODENPUZZLES
