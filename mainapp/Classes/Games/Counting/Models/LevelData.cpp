//
//  LevelData.cpp -- Collection of worksheets
//  TodoSchool on Oct 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LevelData.hpp"
#include <Common/Basic/CommentStream.h>
#include <cocos/cocos2d.h>

#include "Managers/LanguageManager.hpp"

using namespace std;
using namespace cocos2d;


namespace todoschool {
namespace counting {
    
namespace {
    string assetPrefix() { return "Counting"; }
}  // unnamed namespace


vector<int> LevelData::levelIDsFor(const string& languageTag) {
    vector<int> ids;
    
    for (auto levelItem : levels_) {
        auto& key = levelItem.first;
        if (key.first != languageTag) { continue; }

        ids.push_back(key.second);
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
            auto sheetID = random<int>((int)level.begin_index(), (int)level.end_index() - 1);

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
    
    
    string p = LanguageManager::getInstance()->findLocalizedResource("Games/" + assetPrefix() + "/Counting_Levels.tsv");
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

}  // namespace counting
}  // namespace todoschool
