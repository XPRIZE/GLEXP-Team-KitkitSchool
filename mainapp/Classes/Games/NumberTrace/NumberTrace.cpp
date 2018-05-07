//
//  NumberTrace.cpp -- Trace numbers and count insects
//  TodoSchool on Sep 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "NumberTrace.h"
#include "Core/NumberTraceScene.h"
#include "Models/LevelData.h"
#include <Managers/LanguageManager.hpp>

using todoschool::numbertrace::LevelData;

namespace {
}  // unnamed namespace


NumberTrace::NumberTrace() {
}

NumberTrace::NumberTrace(int LevelID,
                         std::function<void()> OnSuccess,
                         std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> NumberTrace::getCandidateLevelIDs() {
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();

    std::vector<int> IDs;
    for (auto ID : LevelData::defaultData().levelIDsFor(Lang))
        IDs.push_back((int)ID);
    
    return IDs;
}

void NumberTrace::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void NumberTrace::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void NumberTrace::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* NumberTrace::createScene() {
    using namespace todoschool::numbertrace;
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto Sheet = LevelData::defaultData().randomSheetFor(Lang, LevelID);
    
    auto It = NumberTraceScene::create();
    It->TheWorksheet.update(Sheet);
    It->LevelID = LevelID;
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    
    return It;
}
