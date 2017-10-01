//
//  LevelData.cpp -- Collection of worksheets
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LevelData.hpp"
#include "../Utils/CommentStream.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


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
    
    Worksheet& LevelData::randomSheetFor(const string& languageTag, int levelID) {
        // NB(xenosoz, 2016): Prepare to fallback to en-US.
        for (auto tag : {languageTag, string("en-US")}) {
            auto key = LevelKey(tag, levelID);
            auto needle = levels_.find(key);
            
            if (needle != levels_.end()) {
                auto& level = needle->second;
                auto sheetID = random<int>(0, (int)level.size() - 1);
                
                return level.at(sheetID);
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
