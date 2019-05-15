//
//  NumberTraceScene.h -- Number tracing game with insects
//  TodoSchool on Apr 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Models/NumberTraceProblem.h"
#include "../Models/NumberTraceWorksheet.h"
#include "../Utils/NumberTraceNS.h"
#include "../Common/CountField/NumberTraceCountField.h"
#include <Common/Controls/TraceField/TraceField.h>


BEGIN_NS_NUMBERTRACE

class NumberTraceScene: public Scene {
    typedef Scene Super;

    Sprite* Background;
    Node* TheGameNode;
    ProgressIndicator* TheProgressBar;
    countfield::CountField* TheCountField;
    tracefield::TraceField* TheTraceField;

public:
    Chain<Worksheet> TheWorksheet;
    Chain<size_t> TheProblemID;
    Chain<Problem> TheProblem;
    
    int LevelID;
    int SheetID;
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    CREATE_FUNC(NumberTraceScene);
    NumberTraceScene();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
    
    void prepareFirstWork();
    void prepareNextWork();
    
    void beginTheWork();
    void handleCountWorkDidBecomeReady();
    void handleTraceWorkDidEnd();
    void handleCountWorkDidEnd();
    
    void handleSuccess();
    void handleFail();
    
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
};

END_NS_NUMBERTRACE
