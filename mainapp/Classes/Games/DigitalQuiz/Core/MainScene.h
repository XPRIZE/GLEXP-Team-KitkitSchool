//
//  MainScene.h -- Pre/post testing
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/Worksheet.h"
#include "../Models/Problem.h"
#include "../Models/SavedState.h"
#include "../Utils/MainNS.h"
#include "../Stages/Stage.h"


BEGIN_NS_DIGITALQUIZ

class MainScene: public Scene {
    typedef Scene Super;

    int ProblemID;
    Chain<Problem> TheProblem;
    Chain<float> TestClock;
    Chain<bool> ClockEnabled;

    Node* TheGameNode;
    Node* TheStageNode;
    Node* TheEraseDataPopup;

public:
    Chain<Worksheet> TheWorksheet;
    Chain<string> TheStudentName;
    Chain<SavedState> TheSavedState;

    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    CREATE_FUNC(MainScene);
    MainScene();

    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();

    void prepareFirstProblem();
    void prepareNthProblem(int N);
    void prepareNextProblem();

    void beginTheProblem();
    void commitTheProgress();
    void writeTheAnswerToLog(const string& Answer);

    // NB(xenosoz, 2016): We don't count for correctness.
    void handleAnyAnswer(const string& Answer);
    void handleSuccess();
    void handleFail();

    Stage* createStageNode();

    void update(float DeltaSeconds) override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    void onEnter() override;
    void onExit() override;
};

END_NS_DIGITALQUIZ
