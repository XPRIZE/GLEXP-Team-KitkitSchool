//
//  NumberTraceScene.cpp -- Number tracing game with insects
//  TodoSchool on Apr 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "NumberTraceScene.h"
#include "../Utils/NumberTraceMainDepot.h"
#include "Managers/StrictLogManager.h"

#include "CCAppController.hpp"

BEGIN_NS_NUMBERTRACE

using namespace todoschool::countfield;
using namespace todoschool::tracefield;


NumberTraceScene::NumberTraceScene()
: Background(nullptr)
, TheGameNode(nullptr)
, TheProgressBar(nullptr)
, TheCountField(nullptr)
, TheTraceField(nullptr)
{
}

bool NumberTraceScene::init() {
    if (!Super::init()) { return false; }

    MainDepot().preloadSoundEffects();

    clearInternals();
    refreshChildNodes();

    return true;
}

void NumberTraceScene::clearInternals() {
    TheWorksheet.OnValueUpdate = [this](Worksheet&) {
        if (!TheProgressBar) { return; }
        TheProgressBar->setMax((int)TheWorksheet().size());
    };

    TheProblemID.OnValueUpdate = [this](size_t&) {
        if (!TheProgressBar) { return; }
        TheProgressBar->setCurrent((int)TheProblemID() - TheWorksheet().beginProblemID() + 1,
                                   false);
    };

    TheProblem.OnValueUpdate = [this](Problem&) {
        MainDepot Depot;
        auto W = TheProblem();
        
        TheTraceField->clear();
        TheTraceField->Placeholder.update(W.TraceText);
        TheCountField->clear();
        TheCountField->TheAssetType = Depot.stringForAssetType(W.TheAssetType);
        TheCountField->AssetCount.update(W.AssetCount);
        
        Depot.soundForNumber((int)W.AssetCount).play();
    };
}

void NumberTraceScene::refreshChildNodes() {
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    int ZOrder = 0;
    
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
        auto It = ProgressIndicator::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width / 2.f, WindowSize.height - It->getContentSize().height));

        It->setMax((int)TheWorksheet().size());

        addChild(It);
        return It;
    }());

    [&] {
        auto It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));

        It->addClickEventListener([this](Ref*) {
            handleFail();
        });

        It->setLocalZOrder(ZOrder++);
        addChild(It);
        return It;
    }();

    
    // NB(xenosoz, 2016): Descendants of the game node.
    
    TheCountField = ([&] {
        CountField* It = CountField::create();
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point::ZERO);
        
        It->OnGoodAssetClicked = [this](CountField*) {
            MainDepot().soundForGoodAssetTouch().play();
        };
        It->OnBadAssetClicked = [this](CountField*) {
            // NB(xenosoz, 2016): Do nothing on clicking bad assets.
        };
        It->OnCountWorkDidBecomeReady = [this](CountField*) {
            handleCountWorkDidBecomeReady();
        };
        It->OnCountWorkDidEnd = [this](CountField*) {
            handleCountWorkDidEnd();
        };

        TheGameNode->addChild(It);
        return It;
    }());

    TheTraceField = ([&] {
        TraceField* It = TraceField::create();
        It->setContentSize(Size(GameSize.width / 2.f, GameSize.height));
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(GameSize.width / 2.f, 0.f));
        It->setLocalZOrder(ZOrder++);
        
        // NB(xenosoz, 2018): Log for future analysis
        auto makeWorkPath = [this] {
            // NB(xenosoz, 2018): TheProblemID is dynamic. That's why I made this as a function.
            stringstream SS;
            SS << "/" << "NumberTrace";
            SS << "/" << "level-" << LevelID << "-" << SheetID;
            SS << "/" << "work-" << TheProblemID();
            return SS.str();
        };

        It->OnEndEditing = [this, makeWorkPath](TraceField*) {
            StrictLogManager::shared()->game_Peek_Answer("NumberTrace", makeWorkPath(), "work-middle", "work-end");
        };
        It->OnTraceWorkDidEnd = [this, makeWorkPath](TraceField*) {
            StrictLogManager::shared()->game_Peek_Answer("NumberTrace", makeWorkPath(), "work-end", "work-end");
            MainDepot().soundForTraceEnd().play();
            handleTraceWorkDidEnd();
        };

        TheGameNode->addChild(It);
        return It;
    }());
}

void NumberTraceScene::prepareFirstWork() {
    TheProblemID.update(TheWorksheet().beginProblemID());
}

void NumberTraceScene::prepareNextWork() {
    TheProblemID.update(TheProblemID() + 1);
}

void NumberTraceScene::beginTheWork() {
    auto W = TheWorksheet().problemByID(TheProblemID());
    
    TheProblem.update(W);

    if (TheTraceField)
        TheTraceField->Enabled.update(false);
}

void NumberTraceScene::handleCountWorkDidBecomeReady() {
    if (TheTraceField)
        TheTraceField->Enabled.update(true);
}

void NumberTraceScene::handleTraceWorkDidEnd() {
    if (TheCountField) {
        TheCountField->Enabled.update(true);
        for (auto obj : TheCountField->CountObjects) {
            obj->glow(true);
        }
    }
    
    if (TheTraceField)
        TheTraceField->Enabled.update(false);
}

void NumberTraceScene::handleCountWorkDidEnd() {
    if (TheProblemID() + 1 >= TheWorksheet().endProblemID()) {
        handleSuccess();
        return;
    }

    prepareNextWork();
    beginTheWork();
}

void NumberTraceScene::handleSuccess() {
    auto CP = CompletePopup::create();
    CP->show(0.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        CCAppController::sharedAppController()->handleGameComplete(1);
//        TodoSchoolBackButton::popGameScene();
    });
}

void NumberTraceScene::handleFail() {
    auto Guard = NodeScopeGuard(this);

    if (OnFail)
        OnFail();
}

void NumberTraceScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    
    prepareFirstWork();
    beginTheWork();
}

void NumberTraceScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

END_NS_NUMBERTRACE
