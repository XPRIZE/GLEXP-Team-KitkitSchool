//
//  Stage.h -- IMAGE_DUMMY for DigitalQuiz
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/DigitalQuizNS.h"
#include "../Models/DigitalQuizProblem.h"


BEGIN_NS_DIGITALQUIZ

class Stage : public Node {
    typedef Node Super;
    
    Label* IndicatorNode;
    Label* StudentNameNode;
    Label* TestClockNode;
    
public:
    Chain<Problem> TheProblem;
    Chain<string> TheStudentName;
    Chain<float> TestClock;

    SoundEffect OpeningSound;
    
    function<void(const string&)> OnAnswer;

public:
    CREATE_FUNC(Stage);
    Stage();
    bool init() override;

protected:
    virtual void clearInternals();
    virtual void refreshChildNodes();

    void onEnter() override;
    void onExit() override;
    void handleAnswer(const string& Answer);
};

END_NS_DIGITALQUIZ
