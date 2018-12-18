//
//  SpellingScene.h -- Spell-filling game with balls
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/SpellingWorksheet.h"
#include "../Models/SpellingProblem.h"
#include "../ChildNodes/SpellingGameBoard.h"
#include "../ChildNodes/Platform.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING

class SpellingScene: public Scene {
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
    
    int LevelID;
    int SheetID;
    function<void()> OnSuccess;
    function<void()> OnFail;
    
    void removeSignLanguageVideo();

public:
    CREATE_FUNC(SpellingScene);
    SpellingScene();

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
