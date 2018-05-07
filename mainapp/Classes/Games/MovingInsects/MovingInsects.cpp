//
//  MovingInsects.cpp -- Learn add and subtraction for small numbers with bugs
//  TodoSchool on Sep 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MovingInsects.h"
#include "Core/MainScene.h"
#include "Models/LevelData.h"

using todoschool::movinginsects::LevelData;


MovingInsects::MovingInsects() {
    
}

MovingInsects::MovingInsects(int LevelID,
                             std::function<void()> OnSuccess,
                             std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> MovingInsects::getCandidateLevelIDs() {
    auto Data = LevelData::defaultData();

    std::vector<int> IDs;
    for (size_t LevelID = Data.beginLevelID(); LevelID < Data.endLevelID(); ++LevelID) {
        IDs.push_back((int)LevelID);
    }

    return IDs;
}

void MovingInsects::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void MovingInsects::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void MovingInsects::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* MovingInsects::createScene() {
    using namespace todoschool::movinginsects;
    
    auto Sheet = LevelData::defaultData().levelByID(LevelID).randomSheet();
    auto It = MainScene::createScene(LevelID);

    It->TheSheet.update(Sheet);
    It->LevelID = LevelID;
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;

    return It;
}
