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


BEGIN_NS_WORDTRACE

vector<int> LevelData::levelIDsFor(const string& LanguageTag) {
    vector<int> IDs;

    for (auto LevelItem : Levels) {
        auto& Key = LevelItem.first;
        if (Key.first != LanguageTag) { continue; }
        IDs.push_back((int)Key.second);
    }
    
    stable_sort(IDs.begin(), IDs.end());
    return IDs;
}

Worksheet& LevelData::randomSheetFor(const string& LanguageTag, size_t LevelID) {
    auto K = LevelKey(LanguageTag, LevelID);
    auto& L = Levels[K];
    
    return L.at(random(L.begin_index(), L.end_index()-1));
}

LevelData LevelData::defaultData() {
    LevelData It;
    
    string P = MainDepot().assetPrefix() + "/WordTrace_Levels.tsv";
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
        auto K = LevelData::LevelKey(LanguageTag, LevelID);
        auto& L = LD.Levels[K];
        
        size_t SheetID;
        if (IS >> SheetID) {
            L.resize_for_index(SheetID);
            auto& S = L.at(SheetID);
            IS >> S;
        }
    }
    
    return IS;
}

END_NS_WORDTRACE