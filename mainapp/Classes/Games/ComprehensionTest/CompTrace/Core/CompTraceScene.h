//
//  CompTraceScene.h -- A game scene for CompTrace
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once


#include <Common/Controls/TraceField/TraceField.h>
#include <functional>

#include "../../ComprehensionScene.hpp"



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
    enum class ProblemType
    {
        IMAGE = 1,
        IMAGE_AND_SOUND = 2
    };
    
    class Problem {
    public:
        std::string Text;
        std::string TeaserFileName;
        
        // added
        ProblemType Type;
        std::string QuestionSound;
        std::string AnswerSound;
    };
    
    

    
    cocos2d::Node* TheGameNode;
    cocos2d::Node* ImageNode;
    
    todoschool::tracefield::TraceField* TheTraceField;


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
    void onExit() override;
    void onExitTransitionDidStart() override;
    
    void writePlayLog();
    std::string makeWorkPath();
};

