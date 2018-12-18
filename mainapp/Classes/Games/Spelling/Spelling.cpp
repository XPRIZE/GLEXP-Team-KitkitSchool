//
//  Spelling.cpp -- Spell-filling game with balls
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Spelling.h"
#include "Core/SpellingScene.h"
#include "Models/SpellingLevelData.h"
#include <Managers/LanguageManager.hpp>
#include <numeric>

using todoschool::spelling::LevelData;


Spelling::Spelling() {
}

Spelling::Spelling(int LevelID,
                   std::function<void()> OnSuccess,
                   std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> Spelling::getCandidateLevelIDs() {
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    return LevelData::defaultData().levelIDsFor(Lang);
}

void Spelling::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void Spelling::setOnSuccess(std::function<void ()> F) {
    this->OnSuccess = F;
}

void Spelling::setOnFail(std::function<void ()> F) {
    this->OnFail = F;
}

cocos2d::Scene* Spelling::createScene() {
    using namespace todoschool::spelling;
    int SheetID;
    
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto Sheet = LevelData::defaultData().randomSheetFor(Lang, LevelID, &SheetID);
    auto It = SpellingScene::create();
    
    It->TheSheet.update(Sheet);
    It->LevelID = LevelID;
    It->SheetID = SheetID;
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    
    return It;
}
