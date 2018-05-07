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

// for data checking
#include "Managers/UserManager.hpp"
#include "3rdParty/CCNativeAlert.h"
#include "Common/Basic/SoundEffect.h"


using namespace std;
using namespace cocos2d;


namespace todoschool {
namespace lettermatching {

namespace {
    string assetPrefix() { return "LetterMatching"; }
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
    
    string p = assetPrefix() + "/LetterMatching_Levels.tsv";
    string s = FileUtils::getInstance()->getStringFromFile(p);
    
    CommentStream stream(s);
    stream >> it;
    
    if (UserManager::getInstance()->isDebugMode()) {
        it.checkData();
    }
    
    return it;
}

    
void LevelData::checkData()
{
    
    string errorList = "";
    
    for (auto it : levels_ ) {
        auto key = it.first;
        auto sheets = it.second;
        auto lang = it.first.first;
        auto level = it.first.second;
        
        for (auto sheet : sheets) {
            for (int i = sheet.beginProblemID(); i<sheet.endProblemID(); i++) {
                auto p = sheet.problemByID(i);
                
                for (auto piece : p.pieces) {

                    SoundEffect sound;
                    sound = sound || SoundEffect(piece.matchSound);
                    sound = sound || SoundEffect("NumberMatching/Sound/" + piece.matchSound);
                    sound = sound || SoundEffect("NumberMatching/Images/Letter/" + piece.matchSound);
                    
                    if (sound.bad()) errorList += "missing sound : ["+piece.matchSound+"] \n";
                    
                    if (piece.matchSound=="star.wav") errorList += "missing sound for : ["+piece.imageNameA+"] \n";
                    
                    {
                        auto s = std::string("NumberMatching/Images/Letter/") + piece.imageNameA;
                        if (!FileUtils::getInstance()->isFileExist(s)) errorList += "missing image : ["+piece.imageNameA+"] \n";
                    }
                    {
                        auto s = std::string("NumberMatching/Images/Letter/") + piece.imageNameB;
                        if (!FileUtils::getInstance()->isFileExist(s)) errorList += "missing image : ["+piece.imageNameB+"] \n";
                    }
          
                }
                
                
            }
        }
    }
    
    if (errorList.length()>0) {
        NativeAlert::show("Error in LetterMatching_Levels.tsv", errorList, "OK");
    }
    
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

}  // namespace lettermatching
}  // namespace todoschool
