//
//  LetterTrace.cpp -- Letter tracing game with bonus videos
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LetterTrace.h"
#include "Core/LetterTraceScene.h"
#include "Models/LevelData.h"
#include <Managers/LanguageManager.hpp>
#include <numeric>


using todoschool::lettertrace::LevelData;

namespace {
    std::string currentLanguageTag() {
        auto CodeToTag = std::map<std::string, std::string>{{"en", "en_US"}, {"sw", "sw_TZ"}};
        auto Code = LanguageManager::getInstance()->getCurrentLanguageCode();
        if (CodeToTag.find(Code) == CodeToTag.end()) {
            return "en_US";
        }
        return CodeToTag.at(Code);
    };
    
}  // unnamed namespace


LetterTrace::LetterTrace() {
}

LetterTrace::LetterTrace(int LevelID,
                         std::function<void()> OnSuccess,
                         std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> LetterTrace::getCandidateLevelIDs() {
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    return LevelData::defaultData().levelIDsFor(Lang);
}

void LetterTrace::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void LetterTrace::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void LetterTrace::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* LetterTrace::createScene() {
    using namespace todoschool::lettertrace;

    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto Sheet = LevelData::defaultData().randomSheetFor(Lang, LevelID);

    auto It = LetterTraceScene::create();
    It->LevelID = LevelID;
    It->TheWorksheet.update(Sheet);
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;

    return It;
}
