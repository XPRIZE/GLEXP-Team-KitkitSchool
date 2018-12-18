//
//  LevelData.hpp -- Collection of worksheets
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "ShapeMatchingWorksheet.hpp"
#include "Shape.hpp"
#include <string>
#include <vector>
#include <map>


namespace ShapeMatching {
    
    class LevelData {
        typedef std::pair<std::string, int> LevelKey;
        typedef std::vector<Worksheet> Level;
        std::map<LevelKey, Level> levels_;
        
    public:
        std::vector<std::string> attribute_keys;
        std::vector<std::string> language_keys;
        std::vector<std::string> group_keys;
        
        // shapes: shape_key -> shape
        std::map<std::string, Shape> shapes;
        
        // groups: group_key_s -> group_key_i -> [shape_key].
        std::map<std::string, std::map<std::string, std::vector<std::string>>> groups;
        
    public:
        std::vector<int> levelIDsFor(const std::string& languageTag);
        Worksheet& randomSheetFor(const std::string& languageTag, int levelID, int *sheetID);
        
    public:
        static LevelData defaultData();
        friend std::istream& operator>>(std::istream& stream, LevelData& data);
    };
    
}  // namespace ShapeMatching
