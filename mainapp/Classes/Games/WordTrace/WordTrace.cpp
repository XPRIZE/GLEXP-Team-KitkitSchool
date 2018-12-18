//
//  WordTrace.cpp -- Word tracing game with bonus video
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WordTrace.h"
#include "Core/WordTraceScene.h"
#include "Models/WordTraceLevelData.h"
#include <Managers/LanguageManager.hpp>


using todoschool::wordtrace::LevelData;

namespace {
}  // unnamed namespace


WordTrace::WordTrace() {
}

WordTrace::WordTrace(int LevelID,
                     std::function<void()> OnSuccess,
                     std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> WordTrace::getCandidateLevelIDs() {
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    return LevelData::defaultData().levelIDsFor(Lang);
}

void WordTrace::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void WordTrace::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void WordTrace::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* WordTrace::createScene() {
    using namespace todoschool::wordtrace;

    size_t SheetID;
    
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto Sheet = LevelData::defaultData().randomSheetFor(Lang, LevelID, &SheetID);
    
    auto It = WordTraceScene::create();
    It->TheWorksheet.update(Sheet);
    It->LevelID = LevelID;
    It->SheetID = SheetID;
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;

    return It;
}
