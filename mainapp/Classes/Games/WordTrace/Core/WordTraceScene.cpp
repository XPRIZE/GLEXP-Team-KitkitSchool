//
//  WordTraceScene.cpp -- A game scene for WordTrace
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "WordTraceScene.h"
#include "../Utils/MainDepot.h"
#include <Games/WoodenPuzzles/Components/TargetDragBody.h>

#include "CCAppController.hpp"


BEGIN_NS_WORDTRACE

using namespace todoschool::tracefield;
using namespace cocos2d;
using namespace std;
using todoschool::woodenpuzzles::TargetDragBody;


namespace {
    bool showDebugLines() {
        // NB(xenosoz, 2016): It's dangerous to go alone! Take this.
        //   (Return true to show draggable, console-debug-printing line.)
        return false;
    }
    
    Vec2 traceFieldOffset() { return Vec2(0.f, 900.f - (1394.f + 614.272f) / 2.f); }
    Point topLineP() {
        return Point(MainDepot().gameSize().width/2.f, 1394.f) + traceFieldOffset();
    }
    Point midLineP() {
        return Point(MainDepot().gameSize().width/2.f, 1171.25f) + traceFieldOffset();
    }
    Point botLineP() {
        return Point(MainDepot().gameSize().width/2.f, 614.272f) + traceFieldOffset();
    }
    
    string solidLineSkin() {
        return MainDepot().assetPrefix() + "/wordtracing_topbottom_line.png";
    }
    string dashedLineSkin() {
        return MainDepot().assetPrefix() + "/wordtracing_middle_line.png";
    }
    string solidDebugSkin() {
        return MainDepot().assetPrefix() + "/wordtracing_topbottom_line_debug.png";
    }
    string dashedDebugSkin() {
        return MainDepot().assetPrefix() + "/wordtracing_middle_line_debug.png";
    }
}  // unnamed namespace


WordTraceScene::WordTraceScene()
: TopGuide(nullptr)
, MidGuide(nullptr)
, BotGuide(nullptr)
, Background(nullptr)
, TheProgressBar(nullptr)
, TheGameNode(nullptr)
, TheTraceField(nullptr)
, TheBonusNode(nullptr)
, TheTrailerNode(nullptr)
, NextWorkIsValid(false)
{
}

bool WordTraceScene::init() {
    if (!Super::init()) { return false; }

    MainDepot().preloadSoundEffects();

    clearInternals();
    refreshChildNodes();
    
    return true;
}

void WordTraceScene::clearInternals() {
    ProgressMaxIndex.OnValueUpdate = [this](int&) {
        if (!TheProgressBar) { return; }
        TheProgressBar->setMax(ProgressMaxIndex());
    };
    ProgressMaxIndex.update(0);

    ProgressIndex.OnValueUpdate = [this](int&) {
        if (!TheProgressBar) { return; }
        TheProgressBar->setCurrent(ProgressIndex(), false);
    };
    ProgressIndex.update(0);

    TheTraceWork.OnValueUpdate = [this](Problem&) {
        MainDepot Depot;
        
        Problem W = TheTraceWork();
        string VideoFilePath = Depot.videoFilePathByFileName(W.VideoFileName);
        
        TheTraceField->Placeholder.update(W.Text);
        TheBonusNode->VideoFilePath.update(VideoFilePath);
        TheBonusNode->Text.update(W.Text);
    };
}

void WordTraceScene::refreshChildNodes() {
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    removeAllChildren();
    
    // NB(xenosoz, 2016): Direct descendants of this.
    
    Background = ([&] {
        Sprite* It = Depot.createBackgroundSprite();
        addChild(It);
        return It;
    }());
    
    TheGameNode = ([&] {
        Node* It = Depot.createGameNode();
        addChild(It);
        return It;
    }());
    
    TheProgressBar = ([&] {
        ProgressIndicator* It = ProgressIndicator::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width / 2.f, WindowSize.height - It->getContentSize().height));
        
        It->setMax(ProgressMaxIndex());
        It->setCurrent(ProgressIndex());
        addChild(It);
        return It;
    }());

    [&] {
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            // NB(xenosoz, 2016): Since releasing VideoPlayer takes time, just hide it first.
            TheBonusNode->setVisible(false);

            handleFail();
        });
        
        addChild(It);
        return It;
    }();

    
    // NB(xenosoz, 2016): Descendants of the game node.
    
    using std::placeholders::_1;

    // NB(xenosoz, 2016): Guide posistion settings are very sensitive.
    //   Font face, font size, stroke thickness ...
    
    TopGuide = [&] {
        auto It = Sprite::create(solidLineSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(topLineP());
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }();
    
    MidGuide = [&] {
        auto It = Sprite::create(dashedLineSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(midLineP());
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }();
    
    BotGuide = [&] {
        auto It = Sprite::create(solidLineSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(botLineP());

        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }();


    TheTraceField = ([&] {
        TraceField* It = TraceField::create();
        It->setContentSize(Size(GameSize.width, GameSize.height * 0.65));
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize / 2.f) + traceFieldOffset());
        It->EnableDefaultBackground.update(false);
        It->ClippingEnabled.update(false);
        
        It->TheStyle.update([&] {
            float MintLineT = 10.f;
            //Color4B MintLineC(156, 206, 167, 255);
            Color4B MintLineC(107, 185, 232, 255);
            
            float WhiteLineT = 80.f;
            //Color4B WhiteLineC(255, 251, 176, GLubyte(255 * .50f));
            Color4B WhiteLineC(255, 249, 237, GLubyte(255 * .90f));
            
            float StrokeLineT = 80.f;
            //Color4B StrokeLineC(81, 53, 24, GLubyte(255 * .90f));
            Color4B StrokeLineC(179, 230, 79, GLubyte(255 * .90f));


            using LT = TraceLineStyle::Type;
            TraceFieldStyle Style;
            
            Style.ScrollLetterByLetter = true;
            Style.MainLineThickness = 130.f;
            
            // NB(xenosoz, 2016): Passive guide and stroke.
            Style.LineStyles.emplace_back(LT::PassiveGuide, WhiteLineT, WhiteLineC);
            Style.LineStyles.emplace_back(LT::PassiveStroke, StrokeLineT, StrokeLineC);
            
            // NB(xenosoz, 2016): Active guide and stroke.
            Style.LineStyles.emplace_back(LT::ActiveGuide, WhiteLineT, WhiteLineC);
            Style.LineStyles.emplace_back(LT::ActiveStroke, StrokeLineT, StrokeLineC);
            
            // NB(xenosoz, 2016): Active guide highlight.
            Style.LineStyles.emplace_back(LT::ActiveGuide, MintLineT, MintLineC);
            
            return Style;
        }());

        It->OnTraceWorkDidEnd = [this](TraceField*) {
            auto Guard = NodeScopeGuard(this);
            handleTraceWorkDidFinish();
        };

        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());
    
    if (showDebugLines())
    {
        [&] {
            auto It = Sprite::create(solidDebugSkin());
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(topLineP());
            
            auto DB = TargetDragBody::create();
            DB->OnDrag.emplace_back([this, It, GameSize](Touch*, Event*) {
                REPR(GameSize);
                REPR(It->getPosition() - traceFieldOffset());
            });
            It->addComponent(DB);
            
            TheGameNode->addChild(It);
            return It;
        }();
        
        [&] {
            auto It = Sprite::create(dashedDebugSkin());
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(midLineP());
            
            auto DB = TargetDragBody::create();
            DB->OnDrag.emplace_back([this, It, GameSize](Touch*, Event*) {
                REPR(GameSize);
                REPR(It->getPosition() - traceFieldOffset());
            });
            It->addComponent(DB);
            
            TheGameNode->addChild(It);
            return It;
        }();
        
        [&] {
            auto It = Sprite::create(solidDebugSkin());
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(botLineP());
            
            auto DB = TargetDragBody::create();
            DB->OnDrag.emplace_back([this, It, GameSize](Touch*, Event*) {
                REPR(GameSize);
                REPR(It->getPosition() - traceFieldOffset());
            });
            It->addComponent(DB);
            
            TheGameNode->addChild(It);
            return It;
        }();
    }
    
    TheBonusNode = ([&] {
        BonusNode* It = BonusNode::create();
        It->setContentSize(GameSize);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize / 2.f));
        
        It->OnBonusWorkDidFinish = [this](BonusNode&) {
            auto Guard = NodeScopeGuard(this);
            handleBonusWorkDidFinish();
        };
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());
    
    TheTrailerNode = ([&] {
        TrailerNode* It = TrailerNode::create();
        It->setContentSize(GameSize);
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point::ZERO);
        
        It->OnNextButtonClicked = [this](TrailerNode&, Problem SelectedWork) {
            auto Guard = NodeScopeGuard(this);

            NextTraceWork.update(SelectedWork);
            NextWorkIsValid = true;
            MainDepot().soundForGoodAssetTouch().play();

            handleTrailerWorkDidFinish();
        };
        
        It->NextTraceWorkA.follow(NextTraceWorkA);
        It->NextTraceWorkB.follow(NextTraceWorkB);
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());
}

void WordTraceScene::prepareFirstWork() {
    // NB(xenosoz, 2016): Clear the candidate problem list.
    ProblemIDsToGo.resize(TheWorksheet().size());
    iota(ProblemIDsToGo.begin(), ProblemIDsToGo.end(), TheWorksheet().beginProblemID());

    ProgressMaxIndex.update(max(0, (int)TheWorksheet().size() - 1));
    ProgressIndex.update(1);
    

    // NB(xenosoz, 2016): Pick a new problem.
    NextWorkIsValid = false;
    if (ProblemIDsToGo.size() < 1) { return; }

    int ProblemID = [&] {
        // NB(xenosoz, 2016): Pick one.
        auto IDs = sample(ProblemIDsToGo, 1);
        assert(IDs.size() == 1);
        return IDs[0];
    }();
    
    auto Work = TheWorksheet().problemByID(ProblemID);
    NextTraceWork.update(Work);
    NextWorkIsValid = true;
}

void WordTraceScene::prepareNextWork() {
    // NB(xenosoz, 2016): Most of the work is done by trailer node itself.
    ProgressIndex.update(ProgressIndex() + 1);
}

void WordTraceScene::beginTraceWork() {
    if (!NextWorkIsValid) {
        // NB(xenosoz, 2016): We have no more work to do -> success!
        handleSuccess();
        return;
    }

    // NB(xenosoz, 2016): Update the work to do.
    TheTraceWork.update(NextTraceWork());
    NextWorkIsValid = false;

    auto ProblemID = TheTraceWork().ProblemID;
    ProblemIDsToGo.erase(remove(ProblemIDsToGo.begin(),
                                ProblemIDsToGo.end(),
                                ProblemID),
                         ProblemIDsToGo.end());


    // NB(xenosoz, 2016): Visual work.
    TheTraceField->setVisible(true);
    TopGuide->setVisible(true);
    MidGuide->setVisible(true);
    BotGuide->setVisible(true);

    TheBonusNode->setVisible(false);
    TheTrailerNode->setVisible(false);
    
    // NB(xenosoz, 2016): The sound.
    float Delay = 0.f;
    const float DelayUnit = 1.2f;

    Delay += (ProgressIndex() > 1 ? DelayUnit : DelayUnit / 2.f);
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
        MainDepot().soundForWord(TheTraceWork().Text).play();
    }, Delay, "WordTraceScene::beginTraceWork::soundForWord");
}

void WordTraceScene::beginBonusWork() {
    TheTraceField->setVisible(false);
    TopGuide->setVisible(false);
    MidGuide->setVisible(false);
    BotGuide->setVisible(false);

    TheBonusNode->setVisible(true);
    TheTrailerNode->setVisible(false);
    
    TheBonusNode->startBonusWork();
    
    // NB(xenosoz, 2016): The Sound.
    float Delay = 0.f;
    const float DelayUnit = 1.2f;
    
    Delay += DelayUnit;
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For sound delay
        MainDepot().soundForWord(TheTraceWork().Text).play();
    }, Delay, "WordTraceScene::beginBonusWork::soundForWord");
}

void WordTraceScene::beginTrailerWork() {
    if (ProblemIDsToGo.size() < 2) {
        // NB(xenosoz, 2016): Not enough problems to choose -> done.
        handleTrailerWorkDidFinish();
        return;
    }

    // NB(xenosoz, 2016): Pick two of them.
    auto IDs = sample(ProblemIDsToGo, 2);
    assert(IDs.size() == 2);

    auto NextWorkA = TheWorksheet().problemByID(IDs[0]);
    auto NextWorkB = TheWorksheet().problemByID(IDs[1]);
    NextTraceWorkA.update(NextWorkA);
    NextTraceWorkB.update(NextWorkB);
    

    // NB(xenosoz, 2016): Visual work
    TheTraceField->setVisible(false);
    TopGuide->setVisible(false);
    MidGuide->setVisible(false);
    BotGuide->setVisible(false);

    TheBonusNode->setVisible(false);
    TheTrailerNode->setVisible(true);
    
    MainDepot().soundForWorkComplete().play();
}

void WordTraceScene::handleTraceWorkDidFinish() {
    beginBonusWork();
}

void WordTraceScene::handleBonusWorkDidFinish() {
    beginTrailerWork();
}

void WordTraceScene::handleTrailerWorkDidFinish() {
    prepareNextWork();
    beginTraceWork();
}

void WordTraceScene::handleSuccess() {
    TheTraceField->setVisible(false);
    TopGuide->setVisible(false);
    MidGuide->setVisible(false);
    BotGuide->setVisible(false);

    TheBonusNode->setVisible(false);
    TheTrailerNode->setVisible(false);

    auto CP = CompletePopup::create();
    CP->show(1.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
  
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

void WordTraceScene::handleFail() {
    auto Guard = NodeScopeGuard(this);
    
    if (OnFail)
        OnFail();
}

void WordTraceScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();

    prepareFirstWork();
    beginTraceWork();
}

void WordTraceScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

END_NS_WORDTRACE
