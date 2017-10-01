//
//  NumberTraceExtScene.h -- A game scene for NumberTraceExt
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once


#include "../Utils/MainNS.h"
#include "../Models/Problem.h"

#include <Common/Controls/TraceField/TraceField.h>
#include <functional>


BEGIN_NS_NUMBERTRACEEXT

// NB(xenosoz, 2016): The general gameflow is as following:
//   1. Play one word trace game
//   2. Show two choices and select one.
//   3. Repeat step one until done.
//

class NumberTraceExtScene: public Layer {
    typedef Layer Super;
    enum class NumberObjectType
    {
        Stick,
        Block,
    };
    
public:
    Sprite* Background;
    
    
    cocos2d::Node* TheGameNode;
    cocos2d::Node* NumberListNode;
    cocos2d::Node* NumberObjectNode;
    cocos2d::Node* GuideNode;
    
    tracefield::TraceField* TheTraceField;

    Chain<Problem> TheTraceWork;
    
    Vec2 _traceFieldOffset;
    
    function<void()> OnSuccess;
    function<void()> OnFail;
    
public:
    CREATE_FUNC(NumberTraceExtScene);
    NumberTraceExtScene();
    bool init() override;
    void onStart();
    void setCurrentLevel(int level);
    void playNumberSound(int number);

private:
    int _currentLevel;
    int _currentProblemIndex;
    
    void initUI();
    void initProblem();
    
    void playCurrentNumberSound();
    
    void refreshGuide();
    void refreshNumberList();
    void refreshNumberObjects();
    
    void handleSuccess();
    void handleFail();

    Node* getNumberObjectNode(int count, NumberObjectType type);
    Node* get5x2LargeBlocksObject();
    
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_NUMBERTRACEEXT
