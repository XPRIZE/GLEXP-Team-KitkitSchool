//
//  MainScene.h -- Spell-filling game with balls
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/Worksheet.h"
#include "../Models/Problem.h"
#include "../ChildNodes/GameBoard.h"
#include "../ChildNodes/Platform.h"
#include "../Utils/MainNS.h"


BEGIN_NS_SPELLING

class MainScene: public Scene {
    typedef Scene Super;
    
    size_t ProblemID;
    Chain<Problem> TheProblem;
    SoundEffect SoundForWord;

    Node* TheGameNode;
    ProgressIndicator* TheProgressBar;

    GameBoard* TheGameBoard;
    Platform* ThePlatform;
    
public:
    Chain<Worksheet> TheSheet;
    
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    CREATE_FUNC(MainScene);
    MainScene();

    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();
    
    void prepareFirstWork();
    bool prepareNextWork();  // NB(xenosoz, 2016): Return true iff we have more work to do.

    void beginTheWork();
    void openingGameBoard();
    void openingWordSound();
    void openingEjectBalls();
    void openingPlatform();
    
    void closingWordSound();
    void closingGameBoard();

    void handleCorrectAnswer();
    void handleSuccess();
    void handleFail();
    
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_SPELLING
