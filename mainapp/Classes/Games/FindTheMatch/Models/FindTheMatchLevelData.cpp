//
//  LevelData.cpp -- Level data for FindTheMatch
//  TodoSchool on Oct 6, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FindTheMatchLevelData.h"
#include "../Utils/FindTheMatchMainDepot.h"
#include <Common/Basic/CommentStream.h>


BEGIN_NS_FINDTHEMATCH

LevelData LevelData::defaultData() {
    LevelData It;
    
    string P = MainDepot().assetPrefix() + "/FindTheMatch_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    CommentStream IS(S);
    IS >> It;
    
    return It;
}

size_t LevelData::size() const {
    return Levels.size();
}

vector<int> LevelData::levelIDs() const {
    vector<int> IDs;

    for (size_t I = Levels.begin_index(), E = Levels.end_index(); I < E; ++I) {
        IDs.push_back((int)I);
    }
    return IDs;
}

Worksheet& LevelData::randomSheetFor(int LevelID, int *SheetID) {
    auto& L = Levels.at(LevelID);
    *SheetID = random<int>((int)L.begin_index() , (int)L.end_index()-1);

    return L.at(*SheetID);
}

istream& operator>>(istream& IS, LevelData& Data) {
    size_t LevelID;
    
    while (IS >> LevelID) {
        Data.Levels.resize_for_index(LevelID);
        auto& Level = Data.Levels.at(LevelID);
        
        size_t SheetID;
        if (IS >> SheetID) {
            Level.resize_for_index(SheetID);
            auto& Sheet = Level.at(SheetID);
            IS >> Sheet;
        }
    }
    
    return IS;
}

END_NS_FINDTHEMATCH
