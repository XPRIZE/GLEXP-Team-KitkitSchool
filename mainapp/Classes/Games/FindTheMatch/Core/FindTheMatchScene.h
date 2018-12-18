//
//  FindTheMatchScene.h on Aug 2, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/FindTheMatchWorksheet.h"
#include "../Utils/FindTheMatchNS.h"
#include "../ChildNodes/FindTheMatchCard.h"
#include "../ChildNodes/FindTheMatchDeckBase.h"
#include "../ChildNodes/FindTheMatchPlayerBase.h"
#include "../ChildNodes/FindTheMatchGameBoard.h"
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Common/Controls/ProgressIndicator.hpp>

BEGIN_NS_FINDTHEMATCH


class FindTheMatchScene: public Scene {
    typedef Scene Super;
    
    size_t WorkID;
    Chain<Problem> TheWork;
    
    Node* TheGameNode;
    ProgressIndicator* TheProgressBar;

    GameBoard* TheGameBoard;
    DeckBase* TheDeckBase;
    PlayerBase* ThePlayerBase;

    Chain<bool> GamePlaying;
    Chain<bool> TouchEnabled;

    list<Card*> AnswerCards;
    
public:
    Chain<Worksheet> TheSheet;

    int LevelID;
    int SheetID;
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    static FindTheMatchScene* createScene();
    
    CREATE_FUNC(FindTheMatchScene);
    FindTheMatchScene();
    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();

    void lockTouchEventOnlyForTheCard(Card* ActiveCard);
    void lockTouchEventForAllCards();
    void releaseTouchEventForAllCards();
    
    void prepareFirstWork();
    bool prepareNextWork();  // NB(xenosoz, 2016): Return true iff the next work is ready.
    void beginTheWork();
    void handleCorrectAnswer();
    void handleSuccess();
    void handleFail();

    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};


END_NS_FINDTHEMATCH
