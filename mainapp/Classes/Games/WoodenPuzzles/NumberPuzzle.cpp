//
//  NumberPuzzle.cpp -- Wooden piece number puzzle
//  TodoSchool on Aug 18, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "NumberPuzzle.h"
#include "Core/FreePuzzleScene.h"
#include "Core/OrderedPuzzleScene.h"
#include "Models/LevelData.h"
#include "Stores/StoreForWorksheet.h"

using todoschool::woodenpuzzles::LevelData;


namespace {
}  // unnamed namespace


NumberPuzzle::NumberPuzzle() {
    
}

NumberPuzzle::NumberPuzzle(int LevelID,
                           std::function<void()> OnSuccess,
                           std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> NumberPuzzle::getCandidateLevelIDs() {
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto LD = LevelData::numberPuzzleData().levelIDsFor(Lang);

    std::vector<int> IDs;
    for (auto ID : LD)
        IDs.push_back((int)ID);

    return IDs;
}

void NumberPuzzle::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void NumberPuzzle::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void NumberPuzzle::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* NumberPuzzle::createScene() {
    using namespace todoschool::woodenpuzzles;

    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto WS = LevelData::numberPuzzleData().randomSheetFor(Lang, LevelID);
    
    
    // NB(xenosoz, 2016): Shuffle or not?
    switch (WS.ThePuzzleType)
    {
        case Worksheet::PuzzleType::RandomOrdered:
            WS.shuffleProblems();
            break;
        default:
            /* Do nothing */
            break;
    }
    
    
    switch (WS.ThePuzzleType)
    {
        case Worksheet::PuzzleType::Ordered: /* Fall through */
        case Worksheet::PuzzleType::RandomOrdered: {
            auto It = OrderedPuzzleScene::create();
            It->LevelID = LevelID;
            It->grabPuzzleStore(new StoreForWorksheet(WS));
            It->OnSuccess = OnSuccess;
            It->OnFail = OnFail;
            
            return It;
        }
        default:
            CCLOGERROR("Unsupported puzzle type in %s", __PRETTY_FUNCTION__);
            // NB(xenosoz, 2016): Fall through: A new life goes on for the default case
            
        case Worksheet::PuzzleType::Free: {
            auto It = FreePuzzleScene::create();
            It->LevelID = LevelID;
            It->grabPuzzleStore(new StoreForWorksheet(WS));
            It->OnSuccess = OnSuccess;
            It->OnFail = OnFail;
            
            return It;
        }
    }
    
    // NB(xenosoz, 2016): Should be unreachable here.
    assert(false);
    return nullptr;
}
