//
//  CompTraceScene.h -- A game scene for CompTrace
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

#include "../../ComprehensionScene.hpp"


BEGIN_NS_COMPTRACE

// NB(xenosoz, 2016): The general gameflow is as following:
//   1. Play one word trace game
//   2. Show two choices and select one.
//   3. Repeat step one until done.
//

class CompTraceScene: public Layer {
    typedef Layer Super;

//    Sprite* TopGuide;
//    Sprite* MidGuide;
//    Sprite* BotGuide;
    
    Sprite* _guide;
    

    
public:
    

    
    cocos2d::Node* TheGameNode;
    cocos2d::Node* ImageNode;
    
    tracefield::TraceField* TheTraceField;


    Chain<Problem> TheTraceWork;

    ComprehensionScene *_comprehensionScene;
    
    Vec2 _traceFieldOffset;
    
    function<void()> OnSuccess;
    function<void()> OnFail;
    
public:
    CREATE_FUNC(CompTraceScene);
    CompTraceScene();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();


    void beginTraceWork();


    void handleTraceWorkDidFinish();


    void handleSuccess();
    void handleFail();
    
    void onEnter() override;
    void onExitTransitionDidStart() override;
};

END_NS_COMPTRACE
