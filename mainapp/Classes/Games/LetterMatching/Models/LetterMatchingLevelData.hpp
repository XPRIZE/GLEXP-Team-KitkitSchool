//
//  LevelData.hpp -- Collection of worksheets
//  TodoSchool on Oct 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "LetterMatchingWorksheet.hpp"
#include <Common/Basic/OneBasedVector.h>
#include <istream>
#include <vector>
#include <map>


namespace todoschool {
namespace lettermatching {

class LevelData {
    typedef std::pair<std::string, int> LevelKey;
    typedef OneBasedVector<Worksheet> Level;
    std::map<LevelKey, Level> levels_;
    
public:
    std::vector<int> levelIDsFor(const std::string& languageTag);
    Worksheet& randomSheetFor(const std::string& languageTag, int levelID, int *sheetID);
    
public:
    static LevelData defaultData();
    
    void checkData();
    
    friend std::istream& operator>>(std::istream& stream, LevelData& data);
};

}  // namespace lettermatching
}  // namespace todoschool
