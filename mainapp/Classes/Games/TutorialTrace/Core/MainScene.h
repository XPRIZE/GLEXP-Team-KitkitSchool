//
//  MainScene.h
//  TodoSchool on Dec 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/Worksheet.h"
#include "../Models/Problem.h"
#include "../Utils/MainNS.h"
#include "../ScratchField/ScratchField.h"


BEGIN_NS_TUTORIALTRACE

class MainScene: public Scene {
    typedef Scene Super;
    
    size_t ProblemID;
    Chain<int> TheProblem;
    vector<SoundEffect> SoundsForTraceEnd;

    Node* TheGameNode;
    ProgressIndicator* TheProgressBar;
    ScratchField* TheScratchField;

public:
    Chain<vector<int>> TheSheet;
    
    int LevelID;
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
    void prepareNextWork();
    void startTheWork();
    void finishTheWork();

    void handleTraceWorkDidEnd();
    void handleSuccess();
    void handleFail();
    
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_TUTORIALTRACE
