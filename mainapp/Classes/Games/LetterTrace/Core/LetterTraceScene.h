//
//  LetterTraceScene.h
//  TodoSchool on Jun 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../ChildNodes/LetterTraceBonusNode.h"
#include "../ChildNodes/LetterTraceTrailerNode.h"
#include "../Models/LetterTraceProblem.h"
#include "../Models/LetterTraceWorksheet.h"
#include "../Utils/LetterTraceNS.h"
#include <Common/Controls/ProgressIndicator.hpp>
#include <Common/Controls/TraceField/TraceField.h>


BEGIN_NS_LETTERTRACE

class LetterTraceScene: public Scene {
    typedef Scene Super;

    Sprite* TopGuide;
    Sprite* MidGuide;
    Sprite* BotGuide;
    
public:
    int LevelID;
    int SheetID;
    Sprite* Background;
    ProgressIndicator* TheProgressBar;

    Node* TheGameNode;
    tracefield::TraceField* TheTraceField;
    BonusNode* TheBonusNode;
    TrailerNode* TheTrailerNode;
    
    Chain<size_t> TraceWorkMaxIndex;
    Chain<size_t> TraceWorkIndex;
    Chain<Worksheet> TheWorksheet;
    Chain<Problem> TheTraceWork;
    Chain<Problem> NextTraceWork;
    
    function<void()> OnSuccess;
    function<void()> OnFail;

    //void removeSignLanguageVideo();
    void drawSignLanguageVideo();
    string getVideoFile(string letter);

public:
    CREATE_FUNC(LetterTraceScene);
    LetterTraceScene();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
    
    void prepareFirstWork();
    bool prepareNextWork();  // NB(xenosoz, 2016): Return true iff we have more work to do.

    void beginTheWork();
    void playBonusVideo();
    void showTrailer();

    void handleTraceWorkMaxIndexValueUpdate(size_t&);
    void handleTraceWorkIndexValueUpdate(size_t&);
    void handleTheTraceWorkValueUpdate(Problem&);
    void handleNextTraceWorkValueUpdate(Problem&);
    
    void handleTraceWorkDidEnd();
    void handleBonusWorkDidEnd();
    void handleTrailerWorkDidEnd();
    
    void handleSuccess();
    void handleFail();
    
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_LETTERTRACE
