//
//  FindTheMatch.cpp on Aug 2, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FindTheMatch.h"
#include "Core/FindTheMatchScene.h"
#include "Models/LevelData.h"
#include <numeric>

using todoschool::findthematch::LevelData;


FindTheMatch::FindTheMatch() {
    
}

FindTheMatch::FindTheMatch(int LevelID,
                     std::function<void()> OnSuccess,
                     std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> FindTheMatch::getCandidateLevelIDs() {
    return LevelData::defaultData().levelIDs();
}

void FindTheMatch::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void FindTheMatch::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void FindTheMatch::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* FindTheMatch::createScene() {
    using namespace todoschool::findthematch;
    
    auto Sheet = LevelData::defaultData().randomSheetFor(LevelID);
    auto It = FindTheMatchScene::createScene();
    
    It->TheSheet.update(Sheet);
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    
    return It;
}