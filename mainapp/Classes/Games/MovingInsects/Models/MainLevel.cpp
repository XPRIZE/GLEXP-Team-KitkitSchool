//
//  MainLevel.cpp on Sep 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "MainLevel.h"


BEGIN_NS_MOVINGINSECTS

MainSheet& MainLevel::sheetByID(size_t SheetID) {
    return Sheets.at(SheetID);
}

MainSheet& MainLevel::randomSheet(size_t *SheetID) {
    *SheetID = random(Sheets.begin_index(), Sheets.end_index()-1);
    return sheetByID(*SheetID);
}

istream& operator>>(istream& IS, MainLevel& Level) {
    size_t SheetID;
    
    if (IS >> SheetID) {
        Level.Sheets.resize_for_index(SheetID);
        auto& Sheet = Level.sheetByID(SheetID);
        IS >> Sheet;
    }
    
    return IS;
}

END_NS_MOVINGINSECTS
