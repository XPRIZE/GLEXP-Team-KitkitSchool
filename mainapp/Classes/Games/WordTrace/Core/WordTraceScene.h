//
//  WordTraceScene.h -- A game scene for WordTrace
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../ChildNodes/BonusNode.h"
#include "../ChildNodes/TrailerNode.h"
#include "../Utils/MainNS.h"
#include "../Models/Problem.h"
#include "../Models/Worksheet.h"

#include <Common/Controls/TraceField/TraceField.h>
#include <functional>


BEGIN_NS_WORDTRACE

// NB(xenosoz, 2016): The general gameflow is as following:
//   1. Play one word trace game
//   2. Show two choices and select one.
//   3. Repeat step one until done.
//

class WordTraceScene: public Scene {
    typedef Scene Super;

    Sprite* TopGuide;
    Sprite* MidGuide;
    Sprite* BotGuide;

public:
    cocos2d::Sprite* Background;
    ProgressIndicator* TheProgressBar;
    
    cocos2d::Node* TheGameNode;
    tracefield::TraceField* TheTraceField;
    BonusNode* TheBonusNode;
    TrailerNode* TheTrailerNode;

    Chain<Worksheet> TheWorksheet;
    Chain<Problem> TheTraceWork;

    bool NextWorkIsValid;
    Chain<Problem> NextTraceWork;
    Chain<Problem> NextTraceWorkA;
    Chain<Problem> NextTraceWorkB;

    // NB(xenosoz, 2016): We give problems to children in random order.
    //   So progress index does *not* directly related to the worksheet.
    Chain<int> ProgressMaxIndex;
    Chain<int> ProgressIndex;
    vector<int> ProblemIDsToGo;
    
    int LevelID;
    function<void()> OnSuccess;
    function<void()> OnFail;
    
public:
    CREATE_FUNC(WordTraceScene);
    WordTraceScene();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
    
    void prepareFirstWork();
    void prepareNextWork();

    void beginTraceWork();
    void beginBonusWork();
    void beginTrailerWork();

    void handleTraceWorkDidFinish();
    void handleBonusWorkDidFinish();
    void handleTrailerWorkDidFinish();

    void handleSuccess();
    void handleFail();
    
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_WORDTRACE
