//
//  Worksheet.h on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Symbol.h"
#include <string>
#include <vector>


namespace todoschool {
namespace starfall {

class Worksheet {
public:
    int TargetHitCount;
    float PreludeDuration;
    float TargetFallingDuration;
    float WordRegenCooltime;

    std::vector<std::string> WordList;
    SymbolSetType EnabledSymbols;

public:
    Worksheet();

public:
    static size_t size();
    static Worksheet forLevelID(size_t LevelID);
    static Worksheet dummyLevel1();
    static Worksheet dummyLevel2();
    static Worksheet dummyLevel3();
    
private:
    static SymbolSetType enabledSymbolsWithWords(const std::vector<std::string>& Words);
};

}  // namespace starfall
}  // namespace todoschool
