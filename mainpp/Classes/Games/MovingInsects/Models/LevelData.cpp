//
//  LevelData.cpp on Sep 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LevelData.h"
#include "../Utils/MainDepot.h"
#include <Common/Basic/CommentStream.h>


BEGIN_NS_MOVINGINSECTS

LevelData LevelData::defaultData() {
    LevelData It;

    string P = MainDepot().assetPrefix() + "/MovingInsects_Levels.tsv";
    string S = FileUtils::getInstance()->getStringFromFile(P);
    
    CommentStream IS(S);
    IS >> It;
    
    return It;
}

size_t LevelData::size() const {
    return Levels.size();
}

size_t LevelData::beginLevelID() const {
    return Levels.begin_index();
}

size_t LevelData::endLevelID() const {
    return Levels.end_index();
}

MainLevel& LevelData::levelByID(size_t LevelID) {
    return Levels.at(LevelID);
}

istream& operator>>(istream& IS, LevelData& Data) {
    size_t LevelID;
    
    while (IS >> LevelID) {
        Data.Levels.resize_for_index(LevelID);
        auto& Level = Data.levelByID(LevelID);
        IS >> Level;
    }
    
    return IS;
}

END_NS_MOVINGINSECTS