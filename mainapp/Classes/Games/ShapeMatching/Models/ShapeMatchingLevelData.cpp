//
//  LevelData.cpp -- Collection of worksheets
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ShapeMatchingLevelData.hpp"
#include "Shape.hpp"
#include "Common/Basic/CommentStream.h"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;
using todoschool::CommentStream;

namespace ShapeMatching {
    
    namespace {
        string assetPrefix() { return "ShapeMatching"; }
    }  // unnamed namespace
    
    
    vector<int> LevelData::levelIDsFor(const string& languageTag) {
        vector<int> ids;
        
        for (auto tag : {languageTag, string("en-US")}) {
            for (auto levelItem : levels_) {
                auto& key = levelItem.first;
                if (key.first != tag) { continue; }
                
                ids.push_back(key.second);
            }
            
            if (!ids.empty())
                break;
        }
        
        stable_sort(ids.begin(), ids.end());
        return ids;
    }
    
    Worksheet& LevelData::randomSheetFor(const string& languageTag, int levelID, int *sheetID) {
        // NB(xenosoz, 2016): Prepare to fallback to en-US.
        for (auto tag : {languageTag, string("en-US")}) {
            auto key = LevelKey(tag, levelID);
            auto needle = levels_.find(key);
            
            if (needle != levels_.end()) {
                auto& level = needle->second;
                *sheetID = random<int>(1, (int)level.size());
                
                return level.at(*sheetID -1);
            }
        }
        
        CCLOGERROR("No proper worksheet found for (%s, %d) in %s",
                   languageTag.c_str(),
                   levelID,
                   __PRETTY_FUNCTION__);
        
        return levels_[LevelKey(languageTag, levelID)].at(0);
    }
    
    LevelData LevelData::defaultData() {
        LevelData it;
        
        string p = assetPrefix() + "/ShapeMatching_Levels.tsv";
        string s = FileUtils::getInstance()->getStringFromFile(p);
        CCLOG("!!!!!!!!!!!! %s", s.c_str());
        
        CommentStream stream(s);
        stream >> it;
        
        return it;
    }
    
    istream& operator>>(istream& stream, LevelData& data) {
        int shape_count;
        int attribute_count;
        int language_count;
        int group_count;
        
        //
        stream >> shape_count >> attribute_count >> language_count >> group_count;
        
        //
        string title;
        stream >> title;
        if (title != "Shape_Key") {
            CCLOGERROR("ERROR: title(%s) is not 'Shape_Key'", title.c_str());
            return stream;
        }

        //
        for (int i = 0; i < attribute_count; ++i) {
            string s;
            stream >> s;
            data.attribute_keys.push_back(s);
        }
        
        for (int i = 0; i < language_count; ++i) {
            string s;
            stream >> s;
            data.language_keys.push_back(s);
        }
        
        for (int i = 0; i < group_count; ++i) {
            string s;
            stream >> s;
            data.group_keys.push_back(s);
        }
        
        //
        for (int i = 0; i < shape_count; ++i) {
            Shape shape(data.attribute_keys, data.language_keys, data.group_keys);
            stream >> shape;
            
            data.shapes[shape.key] = shape;
            
            for (const auto& item : shape.groups) {
                string k = item.first;
                string v = item.second;
                if (v == "None") { continue; }
                
                // groups: group_key_s -> group_key_i -> [shape_key].
                data.groups[k][v].push_back(shape.key);
            }
        }
        
        //
        string languageTag;
        size_t levelID;
        
        while (stream >> languageTag >> levelID)
        {
            auto key = LevelData::LevelKey(languageTag, levelID);
            auto& level = data.levels_[key];
            
            size_t sheetID;
            if (stream >> sheetID) {
                if (sheetID > level.size()) {
                    level.resize(sheetID);
                }
                auto& sheet = level.at(sheetID-1);
                stream >> sheet;
            }
        }
        
        return stream;
    }
    
}  // namespace ShapeMatching
