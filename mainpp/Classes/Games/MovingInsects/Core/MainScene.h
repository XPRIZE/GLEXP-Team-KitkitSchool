//
//  MainScene.h on Sep 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"
#include "../Models/MainSheet.h"
#include "../Models/MainWork.h"
#include "../ChildNodes/Card.h"
#include "../ChildNodes/CountField.h"
#include "../ChildNodes/DeckBase.h"
#include "../ChildNodes/GameBoard.h"
#include "../ChildNodes/PlayerBase.h"


BEGIN_NS_MOVINGINSECTS

class MainScene: public Scene {
    typedef Scene Super;
    
    size_t WorkID;
    Chain<MainWork> TheWork;

    Node* TheGameNode;
    ProgressIndicator* TheProgressBar;

    GameBoard* TheGameBoard;
    DeckBase* TheDeckBase;
    PlayerBase* ThePlayerBase;
    CountField* TheCountField;

    vector<Card*> AnswerCards;
    
public:
    Chain<MainSheet> TheSheet;

    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    static MainScene* createScene(int LevelID);

    CREATE_FUNC(MainScene);
    MainScene();
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
    
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_MOVINGINSECTS