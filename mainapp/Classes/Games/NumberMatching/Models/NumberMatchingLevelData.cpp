//
//  LevelData.cpp -- Collection of worksheets
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "NumberMatchingLevelData.hpp"
#include <Common/Basic/CommentStream.h>
#include <cocos/cocos2d.h>
#include "Managers/LanguageManager.hpp"

using namespace std;
using namespace cocos2d;


namespace todoschool {
namespace numbermatching {

namespace {
    string assetPrefix() { return "NumberMatching"; }
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
            *sheetID = random<int>((int)level.begin_index(), (int)level.end_index()-1);
            
            return level.at(*sheetID);
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
    
    //string p = assetPrefix() + "/NumberMatching_Levels.tsv";
    
    string dataSheet = "/numbermatching_levels.tsv";
    string dataSheetSL = "/numbermatching_levels_sl.tsv";
    string p = assetPrefix() + (LanguageManager::getInstance()->isSignLanguageMode() ? dataSheetSL : dataSheet);
    if (FileUtils::getInstance()->isFileExist(p) == false)
        p = assetPrefix() + dataSheet;
    string s = FileUtils::getInstance()->getStringFromFile(p);
    
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
            level.resize_for_index(sheetID);
            auto& sheet = level.at(sheetID);
            stream >> sheet;
        }
    }
    
    return stream;
}

}  // namespace numbermatching
}  // namespace todoschool
