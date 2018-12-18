//
//  LetterTracingCardScene.h -- A game scene for LetterTracingCard
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once


#include "../Utils/LetterTracingCardNS.h"
#include "../Utils/LetterTracingCardMainDepot.h"
#include "../Models/LetterTracingCardProblem.h"
#include "../ChildNodes/LetterTracingCardCard.h"
#include <Common/Controls/TraceField/TraceField.h>
#include <functional>

BEGIN_NS_LETTERTRACINGCARD

class LetterTracingCardScene: public Layer
{
    typedef Layer Super;
    
public:
    Sprite* Background;
    ProgressIndicator* TheProgressBar;
    cocos2d::Node* TheGameNode;
    
    tracefield::TraceField* TracingCard1;
    tracefield::TraceField* TracingCard2;
    tracefield::TraceField* TracingCard3;
    
    function<void()> OnSuccess;
    function<void()> OnFail;
    
    void onStart();
    
public:
    CREATE_FUNC(LetterTracingCardScene);
    LetterTracingCardScene();
    bool init() override;
    void setCurrentLevel(int level);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
#ifdef IMPORT_TSV_FILE_LETTER_TRACING_CARD
    static std::vector<int> getLevelIDs();
#endif
    
private:
    vector<Card*> _cardVector;
    int _currentLevel;
    int _currentProblemIndex;
    Problem* _currentProblem;
    void initData();
    void initUI();
    void initProblem();
    
    void handleSuccess();
    void handleFail();
#ifdef IMPORT_TSV_FILE_LETTER_TRACING_CARD
    std::vector<std::tuple<string, string, string, string>> _levelData;
#else
    std::vector<std::vector<std::vector<std::tuple<string, string, string>>>> getLevelData();
    
    // temp
    std::vector<std::vector<std::vector<std::tuple<string, string, string>>>> _levelDataEn;
    std::vector<std::vector<std::vector<std::tuple<string, string, string>>>> _levelDataSw;
#endif
};

END_NS_LETTERTRACINGCARD
