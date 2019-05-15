//
//  AlphabetPuzzle.cpp on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "AlphabetPuzzle.h"
#include "Core/FreePuzzleScene.h"
#include "Core/OrderedPuzzleScene.h"
#include "Models/WoodenPuzzlesLevelData.h"
#include "Stores/StoreForWorksheet.h"

using todoschool::woodenpuzzles::LevelData;


namespace {
}  // unnamed namespace


AlphabetPuzzle::AlphabetPuzzle() {
    
}

AlphabetPuzzle::AlphabetPuzzle(int LevelID,
                               std::function<void()> OnSuccess,
                               std::function<void()> OnFail)
: LevelID(LevelID)
, OnSuccess(OnSuccess)
, OnFail(OnFail)
{
}

std::vector<int> AlphabetPuzzle::getCandidateLevelIDs() {
    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto LD = LevelData::alphabetPuzzleData().levelIDsFor(Lang);

    std::vector<int> IDs;
    for (auto ID : LD)
        IDs.push_back((int)ID);

    return IDs;
}

void AlphabetPuzzle::setLevelID(int LevelID) {
    this->LevelID = LevelID;
}

void AlphabetPuzzle::setOnSuccess(std::function<void()> F) {
    this->OnSuccess = F;
}

void AlphabetPuzzle::setOnFail(std::function<void()> F) {
    this->OnFail = F;
}

cocos2d::Scene* AlphabetPuzzle::createScene() {
    using namespace todoschool::woodenpuzzles;

    int WorkSheetID;

    auto Lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto WS = LevelData::alphabetPuzzleData().randomSheetFor(Lang, LevelID, &WorkSheetID);


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

    string Mode = "AlphabetPuzzle";
    switch (WS.ThePuzzleType)
    {
        case Worksheet::PuzzleType::Ordered: /* Fall through */
        case Worksheet::PuzzleType::RandomOrdered: {
            auto It = OrderedPuzzleScene::create(Mode);
            It->grabPuzzleStore(new StoreForWorksheet(Mode, WS));
            It->LevelID = LevelID;
            It->WorkSheetID = WorkSheetID;
            It->OnSuccess = OnSuccess;
            It->OnFail = OnFail;
            
            return It;
        }
        default:
            CCLOGERROR("Unsupported puzzle type in %s", __PRETTY_FUNCTION__);
            // NB(xenosoz, 2016): Fall through: A new life goes on for the default case

        case Worksheet::PuzzleType::Free: {
            auto It = FreePuzzleScene::create(Mode);
            It->grabPuzzleStore(new StoreForWorksheet(Mode, WS));
            It->LevelID = LevelID;
            It->WorkSheetID = WorkSheetID;
            It->OnSuccess = OnSuccess;
            It->OnFail = OnFail;

            return It;
        }
    }
    
    // NB(xenosoz, 2016): Should be unreachable here.
    assert(false);
    return nullptr;
}
