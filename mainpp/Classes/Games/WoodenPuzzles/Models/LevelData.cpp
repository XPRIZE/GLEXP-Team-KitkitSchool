//
//  LevelData.cpp -- A collection of Worksheets
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LevelData.h"
#include "../Utils/WoodenPuzzleDepot.h"
#include <Common/Basic/CommentStream.h>


BEGIN_NS_WOODENPUZZLES

vector<size_t> LevelData::levelIDsFor(const string& LanguageTag) {
    vector<size_t> IDs;
    
    for (auto LevelItem : Levels) {
        auto& Key = LevelItem.first;
        if (Key.first != LanguageTag) { continue; }
        IDs.push_back(Key.second);
    }
    
    stable_sort(IDs.begin(), IDs.end());
    return IDs;
}

Worksheet& LevelData::randomSheetFor(const string& LanguageTag, size_t LevelID) {
    for (auto Tag : {LanguageTag, string("en-US")}) {
        auto K = LevelKey(Tag, LevelID);
        if (Levels.find(K) != Levels.end()) {
            auto& L = Levels[K];
            return L.at(random(L.begin_index(), L.end_index() - 1));
        }
    }

    CCLOGERROR("No proper worksheet found for (%s, %zu) in %s",
               LanguageTag.c_str(),
               LevelID,
               __PRETTY_FUNCTION__);
    return Levels[LevelKey(LanguageTag, LevelID)].at(0);
}

LevelData LevelData::alphabetPuzzleData() {
    LevelData It;

    string P = WoodenPuzzleDepot().assetPrefix() + "/AlphabetPuzzle_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);

    CommentStream IS(S);
    IS >> It;

    return It;
}

LevelData LevelData::numberPuzzleData() {
    LevelData It;
    
    string P = WoodenPuzzleDepot().assetPrefix() + "/NumberPuzzle_Levels.tsv";
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

END_NS_WOODENPUZZLES
