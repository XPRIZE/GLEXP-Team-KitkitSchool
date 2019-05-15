//
//  MovingInsectsScene.h on Sep 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MovingInsectsNS.h"
#include "../Models/MainSheet.h"
#include "../Models/MainWork.h"
#include "../ChildNodes/MovingInsectsCard.h"
#include "../ChildNodes/MovingInsectsCountField.h"
#include "../ChildNodes/MovingInsectsDeckBase.h"
#include "../ChildNodes/MovingInsectsGameBoard.h"
#include "../ChildNodes/MovingInsectsPlayerBase.h"


BEGIN_NS_MOVINGINSECTS

class MovingInsectsScene: public Scene {
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

    int LevelID;
    size_t SheetID;
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    static MovingInsectsScene* createScene(int LevelID);

    CREATE_FUNC(MovingInsectsScene);
    MovingInsectsScene();
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
