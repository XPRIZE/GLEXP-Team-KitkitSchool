//
//  MainScene.cpp -- Pre/post testing
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainScene.h"
#include "../Stages/AssortedStages.h"
#include "../ChildNodes/EraseDataConfirmPopup.h"
#include "../Utils/MainDepot.h"
#include <Managers/LogManager.hpp>
#include <Common/Controls/TodoSchoolBackButton.hpp>

#include "CCAppController.hpp"

BEGIN_NS_DIGITALQUIZ

namespace {
    string contentSkin() {
        return MainDepot().assetPrefix() + "/Background/digitalquiz_background.png";
    }
}  // unnamed namespace


MainScene::MainScene()
: TheGameNode(nullptr)
, TheStageNode(nullptr)
, TheEraseDataPopup(nullptr)
{
}

bool MainScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
};

void MainScene::clearInternals() {
    TheProblem.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
    };
    
    TheSavedState.OnValueUpdate = [this](SavedState&) {
        refreshChildNodes();
    };
    
    TestClock.update(0.f);
    ClockEnabled.update(false);
}

void MainScene::refreshChildNodes() {
    removeAllChildren();
    
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    [&] {
        // NB(xenosoz, 2016): The background node.
        auto BN = Node::create();
        BN->setContentSize(WindowSize);
        
        if (true) {
            auto It = Sprite::create(contentSkin());
            Size SpriteSize = It->getContentSize();
            float Scale = Depot.scaleToCoverWindow(SpriteSize);
            
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(WindowSize / 2.f);
            
            It->setScale(Scale);
            It->setBlendFunc(BlendFunc::DISABLE);
            BN->addChild(It);
        }
        
        addChild(BN);
    }();
    
    TheGameNode = ([&] {
        Size WindowSize = Depot.windowSize();
        Size GameSize = Depot.gameSize();
        float Scale = Depot.scaleToBeContainedInWindow(GameSize);
        
        Node* It = Node::create();
        It->setContentSize(GameSize);
        It->setScale(Scale);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(WindowSize / 2.f);
        
        addChild(It);
        return It;
    }());
    

    // NB(xenosoz, 2016): Direct descendants of TheGameNode
    TheStageNode = [&] {
        auto It = createStageNode();

        It->setOpacity(255 * 0.00f);
        TheGameNode->addChild(It);
        return It;
    }();
    
    TheEraseDataPopup = [&] {
        if (!TheSavedState().Valid || TheSavedState().empty()) {
            return (EraseDataConfirmPopup*)nullptr;
        }

        auto It = EraseDataConfirmPopup::create();
        
        It->Classroom.update(TheSavedState().Classroom);
        It->StudentNumber.update(TheSavedState().StudentNumber);

        if (TheSavedState().ProblemID + 1 >= TheWorksheet().endProblemID()) {
            It->ProblemName.update(string(""));
            It->Finished.update(true);
        }
        else {
            int ProblemID = TheSavedState().ProblemID;
            auto P = TheWorksheet().problemByID(ProblemID);
            It->ProblemName.update(P.Indicator);
            It->Finished.update(false);
        }

        It->OnEraseAndRestart = [this] {
            auto Guard = NodeScopeGuard(this);

            TheSavedState().clear();
            prepareFirstProblem();
            beginTheProblem();
        };
        It->OnResume = [this] {
            auto Guard = NodeScopeGuard(this);
            
            auto N = TheSavedState().savedProblemID();
            auto T = TheSavedState().savedTestClock();

            TestClock.update(T);
            prepareNthProblem(N + 1);
            beginTheProblem();
        };
        It->OnCancel = [this] {
            auto Guard = NodeScopeGuard(this);
            CCAppController::sharedAppController()->handleGameComplete(0);
            //TodoSchoolBackButton::popGameScene();
        };
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);

        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }();
}

void MainScene::prepareFirstProblem() {
    ProblemID = TheWorksheet().beginProblemID();
}

void MainScene::prepareNthProblem(int N) {
    ProblemID = N;
}

void MainScene::prepareNextProblem() {
    ProblemID += 1;
}

void MainScene::beginTheProblem() {
    if (ProblemID >= TheWorksheet().endProblemID()) {
        // NB(xenosoz, 2016): There's no more work to do.
        handleSuccess();
        return;
    }

    auto Problem = TheWorksheet().problemByID(ProblemID);
    TheProblem.update(Problem);

    ClockEnabled.update(true);
    
    if (TheStageNode) {
        auto Action = EaseIn::create(FadeIn::create(.2f), 2.f);

        TheStageNode->runAction(Action);
    }
}

void MainScene::commitTheProgress() {
    TheSavedState().updateProblemID(ProblemID);
    TheSavedState().updateTestClock(TestClock());
}

void MainScene::writeTheAnswerToLog(const string& Answer) {
    Json::Value JV;

    string EventName = "Games/DigitalQuiz";
    auto& S = TheSavedState();

    // NB(xenosoz, 2016): The context
    JV["LanguageTag"] = S.LanguageTag;
    JV["LevelID"] = S.LevelID;
    JV["WorksheetID"] = S.WorksheetID;
    JV["Classroom"] = S.Classroom;
    JV["StudentNumber"] = S.StudentNumber;
    JV["CourseKind"] = S.CourseKind;

    // NB(xenosoz, 2016): The main value
    JV["TestClock"] = TestClock();
    JV["ProblemID"] = ProblemID;
    JV["Result"] = (TheProblem().Answer == Answer ? "Correct" : "Wrong");                    
    JV["CorrectChoice"] = TheProblem().Answer;
    JV["StudentChoice"] = Answer;

    LogManager::getInstance()->logEventJson(EventName, JV);
}

void MainScene::handleAnyAnswer(const string& Answer) {
    commitTheProgress();
    writeTheAnswerToLog(Answer);
    
    if (TheStageNode) {
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(EaseIn::create(FadeOut::create(.2f), 2.f));
        Actions.pushBack(CallFunc::create([this] {
            auto Guard = NodeScopeGuard(this);

            prepareNextProblem();
            beginTheProblem();
        }));

        TheStageNode->stopAllActions();
        TheStageNode->runAction(Sequence::create(Actions));
        return;
    }

    prepareNextProblem();
    beginTheProblem();
}

void MainScene::handleSuccess() {
    auto CP = CompletePopup::create();
    CP->show(0.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        exit(0);
        //CCAppController::sharedAppController()->handleGameComplete(1);
        //TodoSchoolBackButton::popGameScene();
    });
}

void MainScene::handleFail() {
    auto Guard = NodeScopeGuard(this);
    
    if (OnFail)
        OnFail();
}
               
Stage* MainScene::createStageNode() {
    Stage* It = nullptr;
    
    auto& GameType = TheProblem().GameType;
    if (false) { /* NB(xenosoz, 2016): #PRAY_FOR_A_GENERIC_SWITCH */ }
    else if (GameType == "IMAGE_DUMMY") { It = ImageDummyStage::create(); }
    else if (GameType == "LETTER_SOUND") { It = LetterSoundStage::create(); }
    else if (GameType == "SYLLABLES_ID") { It = SyllablesIDStage::create(); }
    else if (GameType == "FAMILIAR_WORD") { It = FamiliarWordStage::create(); }
    else if (GameType == "INVENTED_WORD") { It = InventedWordStage::create(); }
    else if (GameType == "LITERACY_END") { It = LiteracyEndStage::create(); }

    else if (GameType == "NUMBER_DUMMY") { It = NumberDummyStage::create(); }
    else if (GameType == "NUMBER_ID") { It = NumberIDStage::create(); }
    else if (GameType == "COUNT") { It = CountStage::create(); }
    else if (GameType == "ADD") { It = AddStage::create(); }
    else if (GameType == "SUB") { It = SubStage::create(); }
    else if (GameType == "MISSING_NUMBER") { It = MissingNumberStage::create(); }
    else if (GameType == "MATH_END") { It = MathEndStage::create(); }

    if (!It) {
        CCLOGERROR("Not supported GameType(%s) in %s", GameType.c_str(), __PRETTY_FUNCTION__);
        It = Stage::create();
    }

    auto GameSize = MainDepot().gameSize();
    It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    It->setPosition(GameSize / 2.f);

    It->TheProblem.update(TheProblem());
    It->TheStudentName.follow(TheStudentName);
    It->TestClock.follow(TestClock);

    It->OnAnswer = [this](const string& Answer) {
        handleAnyAnswer(Answer);
    };

    return It;
}

void MainScene::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);

    if (ClockEnabled()) {
        TestClock.update(TestClock() + DeltaSeconds);
    }
}

void MainScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    
    if (TheSavedState().empty() || !TheEraseDataPopup) {
        prepareFirstProblem();
        beginTheProblem();
        return;
    }

    TheEraseDataPopup->setVisible(true);
}

void MainScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

void MainScene::onEnter() {
    Super::onEnter();
    scheduleUpdate();
}

void MainScene::onExit() {
    Super::onExit();
    unscheduleUpdate();
}

END_NS_DIGITALQUIZ
