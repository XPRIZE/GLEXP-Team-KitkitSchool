//
//  Stage.cpp -- IMAGE_DUMMY for DigitalQuiz
//  TodoSchool on Nov 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Stage.h"
#include "../ChildNodes/HiddenBackButton.h"
#include "../Utils/DigitalQuizMainDepot.h"
#include <Common/Controls/TodoSchoolBackButton.hpp>


BEGIN_NS_DIGITALQUIZ

namespace {
    string formatClock(float T) {
        int Second = (int)T;
        int Minute = Second / 60;
        int Hour = Minute / 60;
        Second %= 60;
        Minute %= 60;
        
        return format("%02d:%02d:%02d", Hour, Minute, Second);
    }
}  // unnamed namespace


Stage::Stage()
: IndicatorNode(nullptr)
, StudentNameNode(nullptr)
, TestClockNode(nullptr)
{
}

bool Stage::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void Stage::clearInternals() {
    MainDepot Depot;
    
    setContentSize(Depot.gameSize());
    setCascadeOpacityEnabled(true);
    
    TheProblem.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
    };
    
    TheStudentName.OnValueUpdate = [this](string&) {
        if (!StudentNameNode) { return; }
        StudentNameNode->setString(TheStudentName());
    };
    
    TestClock.OnValueUpdate = [this](float&) {
        if (!TestClockNode) { return; }
        TestClockNode->setString(formatClock(TestClock()));
    };
}

void Stage::refreshChildNodes() {
    removeAllChildren();
    MainDepot Depot;
    
    Size CS = getContentSize();
    
    [&] {
        // NB(xenosoz, 2016): The hidden back button
        auto It = HiddenBackButton::create();
        It->OnClick = [this] {
            auto Guard = NodeScopeGuard(this);
            TodoSchoolBackButton::popGameScene();
        };
        
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(0.f, CS.height));
        
        addChild(It);
        return It;
    }();
    
    IndicatorNode = [&] {
        auto& Indicator = TheProblem().Indicator;
        auto It = Label::createWithTTF(Indicator,
                                       Depot.defaultFont(), Depot.defaultFontSize());
        It->setColor(Depot.defaultFontColor());
        
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(50.f, CS.height - 20.f));
        addChild(It);
        return It;
    }();
    
    StudentNameNode = [&] {
        auto It = Label::createWithTTF(TheStudentName(),
                                       Depot.defaultFont(), Depot.defaultFontSize());
        It->setColor(Depot.defaultFontColor());
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(CS.width / 2.f, CS.height - 20.f));
        addChild(It);
        return It;
    }();
    
    TestClockNode = [&] {
        auto It = Label::createWithTTF(formatClock(TestClock()),
                                       Depot.defaultFont(), Depot.defaultFontSize());
        It->setColor(Depot.defaultFontColor());
        
        It->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        It->setPosition(Point(CS.width - 50.f, CS.height - 20.f));
        addChild(It);
        return It;
    }();
    
    OpeningSound = [&] {
        auto S = TheProblem().OpeningSound;
        if (S.empty() || S == "__NO_SOUND__") { return SoundEffect(); }

        auto SoundPath = Depot.assetPrefix() + "/QuizSounds/" + S;
        return SoundEffect(SoundPath).preloaded();
    }();
}

void Stage::onEnter() {
    Super::onEnter();
    
    if (OpeningSound)
    {
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(.2f));
        Actions.pushBack(CallFunc::create([this] {
            if (OpeningSound)
                OpeningSound.play();
        }));
        runAction(Sequence::create(Actions));
    }
}

void Stage::onExit() {
    Super::onExit();
    
    if (OpeningSound)
        OpeningSound.stop();
}

void Stage::handleAnswer(const string& Answer) {
    auto Guard = NodeScopeGuard(this);
    
    if (OnAnswer)
        OnAnswer(Answer);
}


END_NS_DIGITALQUIZ
