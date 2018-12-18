//
//  TutorialTraceScene.cpp
//  TodoSchool on Dec 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TutorialTraceScene.h"
#include "Problems.h"
#include "../Utils/TutorialTraceMainDepot.h"
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include "Common/Components/TargetDragBody.h"
#include <Managers/StrictLogManager.h>

#include "CCAppController.hpp"


BEGIN_NS_TUTORIALTRACE


using tracefield::TraceLineStyle;

namespace {
    string contentSkin(int Problem) {
        stringstream SS;
        SS << MainDepot().assetPrefix();
        SS << "/Images/" << Problem;
        SS << "/tutorialtracing" << Problem << "_background.png";
        return SS.str();
    }
}  // unnamed namespace


TutorialTraceScene::TutorialTraceScene()
: TheGameNode(nullptr)
, TheProgressBar(nullptr)
{
}

bool TutorialTraceScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
};


void TutorialTraceScene::clearInternals() {
    auto appendSound = [&](const string& Name) {
        auto S = SoundEffect("Common/Sounds/Effect" + Name).preloaded();
        SoundsForTraceEnd.push_back(S);
    };
    appendSound("/Cards_1.m4a");
    appendSound("/Cards_2.m4a");
    appendSound("/Cards_3.m4a");
    appendSound("/Cards_4.m4a");
    appendSound("/Cards_5.m4a");
    appendSound("/Cards_6.m4a");

    TheProblem.OnValueUpdate = [this](int &) {
        refreshChildNodes();
    };
}

void TutorialTraceScene::refreshChildNodes() {
    removeAllChildren();
    
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    [&] {
        // NB(xenosoz, 2016): The background node.
        auto It = Sprite::create(contentSkin(TheProblem()));
        if (!It) { It = Sprite::create(contentSkin(1)); }
        Size SpriteSize = It->getContentSize();
        float Scale = Depot.scaleToCoverWindow(SpriteSize);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(WindowSize/2.f);
        
        It->setScale(Scale);
        It->setBlendFunc(BlendFunc::DISABLE);
        addChild(It);
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
    
    TheProgressBar = [&] {
        // NB(xenosoz, 2016): The progress indicator.
        auto It = ProgressIndicator::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width / 2.f, WindowSize.height - It->getContentSize().height));
        
        It->setMax((int)TheSheet().size());
        It->setCurrent((int)ProblemID - 0 + 1, false);
        
        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): The back button.
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            handleFail();
        });
        
        addChild(It);
    }();


    // NB(xenosoz, 2016): Direct descendants of TheGameNode

    TheScratchField = [&] {
        auto It = ScratchField::create();
        It->setContentSize(Size(GameSize.width, GameSize.height));
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        It->EnableDefaultBackground.update(false);
        It->ClippingEnabled.update(false);

        It->TheStyle.update(([&] {
            // NB(xenosoz, 2016): The default style.
            using LT = TraceLineStyle::Type;
            TraceFieldStyle Style;
            
            float Thickness = 0.f;
            Color4B Color(255, 255, 255, 0);
            
            Style.ScrollLetterByLetter = false;
            Style.MainLineThickness = Thickness;
            Style.LineStyles.emplace_back(LT::PassiveGuide, Thickness, Color);
            Style.LineStyles.emplace_back(LT::PassiveStroke, Thickness, Color);
            Style.LineStyles.emplace_back(LT::ActiveGuide, Thickness, Color);
            Style.LineStyles.emplace_back(LT::ActiveStroke, Thickness, Color);
            
            return Style;
        })());
        
        setupFieldForProblem(It, TheProblem());
        It->OnEndEditing = [this](ScratchField*) {
            // NB(xenosoz, 2018): Log for future analysis (#1/2)
            string workPath = [this] {
                stringstream SS;
                SS << "/" << "TutorialTrace";
                SS << "/" << "level-" << LevelID << "-0";
                SS << "/" << "work-" << ProblemID;
                return SS.str();
            }();
            StrictLogManager::shared()->game_Peek_Answer("TutorialTrace", workPath, "work-middle",  "work-end");
        };
        It->OnTraceWorkDidEnd = [this](ScratchField*) {
            auto Guard = NodeScopeGuard(this);
            handleTraceWorkDidEnd();
            
            // NB(xenosoz, 2018): Log for future analysis (#2/2)
            string workPath = [this] {
                stringstream SS;
                SS << "/" << "TutorialTrace";
                SS << "/" << "level-" << LevelID << "-0";
                SS << "/" << "work-" << ProblemID;
                return SS.str();
            }();
            StrictLogManager::shared()->game_Peek_Answer("TutorialTrace", workPath, "work-end",  "work-end");
        };
        
        TheGameNode->addChild(It);
        return It;
    }();


#if false
    [&] {
        auto It = Sprite::create(Depot.assetPrefix() + "/Images/5/tutorialtracing5_img.png");
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        
        It->setOpacity(255 * .3f);
        
        TheGameNode->addChild(It);
        return It;
    }();

    
    [&] {
        auto It = Node::create();
        It->setContentSize(Size(602.f, 602.f));
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(450.f, 1002.f));

        auto LineA = Sprite::create(Depot.assetPrefix() +
                                 "/Images/5/tutorialtracing5_line1bottom.png");
        LineA->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        LineA->setPosition(Point(0.f, 0.f));
        It->addChild(LineA);

        auto LineB = Sprite::create(Depot.assetPrefix() +
                                 "/Images/5/tutorialtracing5_line2bottom.png");
        LineB->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        LineB->setPosition(Point(0.f, 0.f));
        It->addChild(LineB);


        auto DB = TargetDragBody::create();
        DB->OnDrag.emplace_back([this, It](Touch*, Event*) {
            REPR(It->getPosition());
        });
        It->addComponent(DB);

        TheGameNode->addChild(It);
        return It;
    }();
#endif
}

void TutorialTraceScene::prepareFirstWork()
{
    ProblemID = 0;
    
    if (ProblemID < TheSheet().size())
        TheProblem.update(TheSheet()[ProblemID]);

    if (TheProgressBar)
        TheProgressBar->setCurrent((int)ProblemID - 0 + 1, false);
}

void TutorialTraceScene::prepareNextWork()
{
    ProblemID = min(ProblemID + 1, TheSheet().size());

    if (ProblemID < TheSheet().size())
        TheProblem.update(TheSheet()[ProblemID]);

    if (TheProgressBar)
        TheProgressBar->setCurrent((int)ProblemID - 0 + 1, false);
}

void TutorialTraceScene::startTheWork() {
    if (ProblemID >= TheSheet().size()) {
        handleSuccess();
        return;
    }
    
    // NB(xenosoz, 2016): Do nothing.
}

void TutorialTraceScene::finishTheWork() {
    if (TheProgressBar)
        TheProgressBar->setCurrent((int)ProblemID - 0 + 1, true);
    
    if (TheScratchField)
        TheScratchField->Enabled.update(false);

    if (SoundsForTraceEnd.size() > 0)
        SoundsForTraceEnd[random((size_t)1, SoundsForTraceEnd.size()) - 1].play();
}

void TutorialTraceScene::handleTraceWorkDidEnd() {
    finishTheWork();

    Vector<FiniteTimeAction*> Actions;
    Actions.pushBack(DelayTime::create(.5f));
    Actions.pushBack(CallFunc::create([this] {
        auto Guard = NodeScopeGuard(this);
        prepareNextWork();
        startTheWork();
    }));
    stopAllActions();
    runAction(Sequence::create(Actions));
}

void TutorialTraceScene::handleSuccess() {
    auto CP = CompletePopup::create();
    CP->show(.2f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        CCAppController::sharedAppController()->handleGameComplete(1);
//        TodoSchoolBackButton::popGameScene();
    });
}

void TutorialTraceScene::handleFail() {
    auto Guard = NodeScopeGuard(this);
    
    if (OnFail)
        OnFail();
}

void TutorialTraceScene::onEnter() {
    Super::onEnter();
    prepareFirstWork();
}

void TutorialTraceScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    startTheWork();
}

void TutorialTraceScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}


END_NS_TUTORIALTRACE
