//
//  LevelData.cpp -- A collection of Worksheets
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellingLevelData.h"
#include "../Utils/SpellingMainDepot.h"
#include <Common/Basic/CommentStream.h>


BEGIN_NS_SPELLING

size_t LevelData::size() const {
    return Levels.size();
}

vector<int> LevelData::levelIDsFor(const string& LanguageTag) {
    vector<int> IDs;
    
    for (auto LevelItem : Levels) {
        auto& Key = LevelItem.first;
        if (Key.first != LanguageTag) { continue; }
        
        IDs.push_back(Key.second);
    }
    
    stable_sort(IDs.begin(), IDs.end());
    return IDs;
}

Worksheet& LevelData::randomSheetFor(const string& LanguageTag, int LevelID, int *SheetID) {
    // NB(xenosoz, 2016): Prepare to fallback to en-US.
    for (auto Tag : {LanguageTag, string("en-US")}) {
        auto Key = LevelKey(Tag, LevelID);
        auto Needle = Levels.find(Key);
        
        if (Needle != Levels.end()) {
            auto& Level = Needle->second;
            *SheetID = random(Level.begin_index(), Level.end_index()-1);
            
            return Level.at(*SheetID);
        }
    }
    
    CCLOGERROR("No proper worksheet found for (%s, %d) in %s",
               LanguageTag.c_str(),
               LevelID,
               __PRETTY_FUNCTION__);
    
    return Levels[LevelKey(LanguageTag, LevelID)].at(0);
}

LevelData LevelData::defaultData() {
    LevelData It;
    
    string P = MainDepot().assetPrefix() + "/Spelling_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    CommentStream IS(S);
    IS >> It;
    
    return It;
}

istream& operator>>(istream& IS, LevelData& LD) {
    string LanguageTag;
    size_t LevelID;

    while (IS >> LanguageTag >> LevelID)
    {
        auto Key = LevelData::LevelKey(LanguageTag, LevelID);
        auto& L = LD.Levels[Key];
        
        size_t SheetID;
        if (IS >> SheetID) {
            L.resize_for_index(SheetID);
            auto& S = L.at(SheetID);
            IS >> S;
        }
    }

    return IS;
}

END_NS_SPELLING
