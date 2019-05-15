//
//  StarFallScene.h on Jul 22, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/StarFallNS.h"
#include "../Models/Symbol.h"
#include "../Models/StarFallWorksheet.h"
#include "../ChildNodes/KeyboardNode.h"
#include "../ChildNodes/ActiveTextNode.h"
#include "../ChildNodes/RocketNode.h"
#include "../ChildNodes/TargetTextNode.h"
#include "../ChildNodes/TargetDummyNode.h"


BEGIN_NS_STARFALL

class StarFallScene: public Scene {
    typedef Scene Super;

public:
    Node* TheGameNode;
    list<TargetTextNode*> TargetTextNodes;
    list<TargetDummyNode*> TargetDummyNodes;
    KeyboardNode* TheKeyboardNode;
    ActiveTextNode* TheActiveTextNode;
    RocketNode* TheRocketNode;
    Label* TempTimer;
    Label* HitCounterNode;
    
    Worksheet TheWorksheet;
    
    Chain<int> TargetHitCount;
    Chain<float> TargetFallingDuration;
    Chain<float> WordRegenCooltime;
    Chain<vector<string>> WordList;
    Chain<SymbolSetType> EnabledSymbols;
    
    Chain<bool> GamePlaying;
    Chain<float> GameClock;
    Chain<float> WordRegenClock;
    Chain<int> BirthCount;
    Chain<int> HitCount;
    Chain<int> MissCount;

    Chain<string> ActiveText;
    Chain<string> BadText;
    
public:
    int LevelID;
    function<void()> OnSuccess;
    function<void()> OnFail;

public:
    CREATE_FUNC(StarFallScene);
    StarFallScene();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
    
    void prepareTheWork();
    void beginTheWork();
    
    void handleSuccess();
    void handleFail();

    void appendNewTargetTextNode();
    void appendNewTargetDummyNode(Point Position, const string& TitleText);
    void refreshActiveTextForSanity();
    
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExit() override;
    void onExitTransitionDidStart() override;
    void update(float DeltaSeconds) override;
};

END_NS_STARFALL
