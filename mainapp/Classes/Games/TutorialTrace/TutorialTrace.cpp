//
//  TutorialTrace.cpp -- Shape tracing for Pre-K students
//  TodoSchool on Dec 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TutorialTrace.h"
#include "Core/MainScene.h"
#include "Models/LevelData.h"
#include <Managers/LanguageManager.hpp>
#include <numeric>

//using todoschool::tutorialtrace::LevelData;


TutorialTrace::TutorialTrace() {
}

TutorialTrace::TutorialTrace(int LevelID,
                             std::function<void()> OnSuccess,
                             std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> TutorialTrace::getCandidateLevelIDs() {
    return std::vector<int>{1, 2, 3};
}

void TutorialTrace::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void TutorialTrace::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void TutorialTrace::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* TutorialTrace::createScene() {
    using namespace todoschool::tutorialtrace;
    
    //auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto Sheet = [&] {
        switch (LevelID) {
            case 1: return vector<int>{1, 2, 3, 4, 5, 6};
            case 2: return vector<int>{7, 8, 9, 10, 11, 12, 13};
            case 3: return vector<int>{14, 15, 16, 17, 18, 19};
            default: {
                CCLOGERROR("Unknown levelID(%d) in %s", LevelID, __PRETTY_FUNCTION__);
                return vector<int>{5};
            }
        }
    }();
    
    auto It = MainScene::create();
    It->TheSheet.update(Sheet);
    It->OnSuccess = OnSuccess;
    It->OnFail = OnFail;
    return It;
}
