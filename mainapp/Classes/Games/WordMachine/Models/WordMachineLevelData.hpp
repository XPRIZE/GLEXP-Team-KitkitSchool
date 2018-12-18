//
//  LevelData.hpp -- Collection of worksheets
//  TodoSchool on Oct 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "WordMachineWorksheet.hpp"
#include <Common/Basic/OneBasedVector.h>
#include <istream>
#include <vector>
#include <map>


namespace todoschool {
namespace wordmachine {

class LevelData {
    typedef std::pair<std::string, int> LevelKey;
    typedef OneBasedVector<Worksheet> Level;

    
public:
    std::map<LevelKey, Level> levels_;
    
    std::vector<int> levelIDsFor(const std::string& languageTag);
    Worksheet& randomSheetFor(const std::string& languageTag, int levelID, int *sheetID);
    
public:
    static LevelData defaultData();
    friend std::istream& operator>>(std::istream& stream, LevelData& data);
};

}  // namespace wordmachine
}  // namespace todoschool
