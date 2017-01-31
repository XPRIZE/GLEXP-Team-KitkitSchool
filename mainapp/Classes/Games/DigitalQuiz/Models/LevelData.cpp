//
//  LevelData.cpp -- A collection of Worksheets
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LevelData.h"
#include "../Utils/MainDepot.h"
#include <Common/Basic/CommentStream.h>
#include <Common/Basic/CustomFacet.h>


BEGIN_NS_DIGITALQUIZ

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

Worksheet& LevelData::randomSheetFor(const string& LanguageTag, int LevelID) {
    // NB(xenosoz, 2016): Prepare to fallback to en-US.
    for (auto Tag : {LanguageTag, string("en-US")}) {
        auto Key = LevelKey(Tag, LevelID);
        auto Needle = Levels.find(Key);
        
        if (Needle != Levels.end()) {
            auto& Level = Needle->second;
            auto SheetID = random(Level.begin_index(), Level.end_index() - 1);
            
            return Level.at(SheetID);
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
    
    string P = MainDepot().assetPrefix() + "/DigitalQuiz_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    CommentStream IS(S);
    auto Facet = [&] {
        auto It = new CustomFacet(IS.getloc());

        // NB(xenosoz, 2016): U+0020 'SPACE' is not a whitespace anymore.
        It->clearSpaceFlagsFor(" ");
        return It;
    }();

    auto Loc = locale(IS.getloc(), Facet);
    IS.imbue(Loc);
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

END_NS_DIGITALQUIZ
