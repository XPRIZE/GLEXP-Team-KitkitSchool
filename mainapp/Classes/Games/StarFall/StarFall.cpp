//
//  StarFall.cpp -- Bubble-falling typing game with a keyboard
//  TodoSchool on Jul 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "StarFall.h"
#include "Models/StarFallWorksheet.h"
#include "Core/StarFallScene.h"
#include <numeric>

using todoschool::starfall::Worksheet;


StarFall::StarFall() {
}

StarFall::StarFall(int LevelID,
                   std::function<void()> OnSuccess,
                   std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> StarFall::getCandidateLevelIDs() {
    std::vector<int> IDs(Worksheet::size());
    std::iota(IDs.begin(), IDs.end(), 1);
    
    return IDs;
}

void StarFall::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void StarFall::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void StarFall::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* StarFall::createScene() {
    using namespace todoschool::starfall;
    
    auto Sheet = Worksheet::forLevelID(LevelID);
    auto It = StarFallScene::create();

    It->TheWorksheet = Sheet;
    It->LevelID = LevelID;
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    
    return It;
}
