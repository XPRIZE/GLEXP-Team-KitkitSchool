//
//  StarFallScene.cpp on Jul 22, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "StarFallScene.h"
#include "../Models/Worksheet.h"
#include "../Utils/StarFallDepot.h"

#include "CCAppController.hpp"

BEGIN_NS_STARFALL

namespace {
    int rocketNodeZOrder() { return -1; }
}  // unnamed namespace


StarFallScene::StarFallScene()
: TheGameNode(nullptr)
, TheKeyboardNode(nullptr)
, TheActiveTextNode(nullptr)
, TheRocketNode(nullptr)
, TempTimer(nullptr)
, HitCounterNode(nullptr)
{
}

bool StarFallScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}
    
void StarFallScene::clearInternals() {
    StarFallDepot Depot;
    Depot.preloadSoundEffects();

    // NB(xenosoz, 2016): Default values.
    TargetHitCount.update(10);
    TargetFallingDuration.update(10.f);
    WordRegenCooltime.update(2.f);
    
    GamePlaying.update(false);
    GameClock.update(0.f);
    WordRegenClock.update(0.f);
    BirthCount.update(0.f);
    HitCount.update(0.f);
    MissCount.update(0.f);

    ActiveText.update("");
    BadText.update("");
    
    GameClock.OnValueUpdate = [this](float&) {
        if (TempTimer) {
            int MS = (int)((GameClock() - floor(GameClock())) * 100);
            int M = (int)GameClock() / 60;
            int S = (int)GameClock() % 60;

            string Text = StringUtils::format("%02d:%02d.%03d", M, S, MS);
            TempTimer->setString(Text);
        }
    };
    
    HitCount.OnValueUpdate =
    TargetHitCount.OnValueUpdate = [this](int&) {
        if (HitCounterNode) {
            auto S = format("%d / %d", HitCount(), TargetHitCount());
            HitCounterNode->setString(S);
        }

        if (HitCount() >= TargetHitCount()) {
            auto Guard = NodeScopeGuard(this);
            handleSuccess();
        }
    };
    
    WordRegenClock.OnValueUpdate = [this](float&) {
        if (!GamePlaying()) { return; }
        if (WordRegenClock() <= WordRegenCooltime()) { return; }

        // NB(xenosoz, 2016): Target birth sound's a little bit annoying.
        //StarFallDepot().soundForTargetBirth().play();

        appendNewTargetTextNode();
        WordRegenClock.update(0.f);
    };
}

void StarFallScene::refreshChildNodes() {
    StarFallDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    removeAllChildren();
    TargetTextNodes.clear();
    TargetDummyNodes.clear();


    // NB(xenosoz, 2016): Direct descendants of this.

    if (true) {
        auto It = Depot.createBackgroundNode();
        addChild(It);
    }
    
    TheGameNode = ([&] {
        Node* It = Depot.createGameNode();
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
        
        addChild(It);
        return It;
    }();


    // NB(xenosoz, 2016): Direct descendants of the game node.

    TheKeyboardNode = ([&] {
        KeyboardNode* It = KeyboardNode::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 0.f));
        
        It->EnabledSymbols.follow(EnabledSymbols);
        It->OnKeyPressed = [this, It](KeyboardNode&, std::string Symbol) {
            ActiveText.update(ActiveText() + Symbol);
            refreshActiveTextForSanity();
        };
        
        TheGameNode->addChild(It);
        return It;
    }());
    
    TheActiveTextNode = ([&] {
        ActiveTextNode* It = ActiveTextNode::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, TheKeyboardNode->getContentSize().height));
        
        It->ActiveText.follow(ActiveText);
        It->BadText.follow(BadText);
        
        TheGameNode->addChild(It);
        return It;
    }());

    TheRocketNode = ([&] {
        auto It = RocketNode::create();
        Point P = Point(GameSize.width / 2.f,
                        TheKeyboardNode->getContentSize().height +
                        TheActiveTextNode->getContentSize().height);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(P);

        It->BasePosition.update(P);
        TheGameNode->addChild(It, rocketNodeZOrder());
        return It;
    }());
    
    TempTimer = ([&] {
        Label* It = Label::createWithTTF("00:00.00", Depot.defaultFontFace(), 50.f);
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(GameSize.width - 300.f, GameSize.height - 50.f));
        
        // NB(xenosoz, 2016): This timer is used for debugging purpose only.
        It->setVisible(false);
        TheGameNode->addChild(It);
        return It;
    }());

    HitCounterNode = ([&] {
        auto S = format("%d / %d", HitCount(), TargetHitCount());
        Label* It = Label::createWithTTF(S, Depot.defaultFontFace(), 80.f);
        It->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        It->setPosition(Point(GameSize.width - 100.f, GameSize.height - 50.f));
        
        TheGameNode->addChild(It);
        return It;
    }());
}

void StarFallScene::prepareTheWork() {
    auto& WS = TheWorksheet;

    TargetHitCount.update(WS.TargetHitCount);
    TargetFallingDuration.update(WS.TargetFallingDuration);
    WordRegenCooltime.update(WS.WordRegenCooltime);
    
    EnabledSymbols.update(WS.EnabledSymbols);
    WordList.update(WS.WordList);
}

void StarFallScene::beginTheWork() {
    // NB(xenosoz, 2016): Make the first word to be generated faster.
    auto& WS = TheWorksheet;

    float T = WS.WordRegenCooltime - WS.PreludeDuration;
    T = max(0.f, min(T, WS.WordRegenCooltime));
    WordRegenClock.update(T);

    GamePlaying.update(true);
}

void StarFallScene::handleSuccess() {
    GamePlaying.update(false);
    
    auto CP = CompletePopup::create();
    CP->show(1.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        CCAppController::sharedAppController()->handleGameComplete(1);
    });
}

void StarFallScene::handleFail() {
    if (OnFail)
        OnFail();
}

void StarFallScene::appendNewTargetTextNode() {
    if (!TheGameNode) { return; }

    StarFallDepot Depot;
    Size GameSize = Depot.gameSize();

    float PointX = random(100.f, GameSize.width - 100.f);
    float PointY = (TheKeyboardNode ? TheKeyboardNode->getContentSize().height : 0.f);
    string Title = ([&] {
        if (WordList().empty()) { return string(); }

        size_t TrialCount = 20;
        for (size_t TrialID = 0; TrialID < TrialCount; ++TrialID) {
            size_t WordID = random((size_t)0, WordList().size() - 1);
            string Word = WordList()[WordID];
            
            bool Collision = false;
            for (TargetTextNode* TextNode : TargetTextNodes) {
                if (TextNode->TitleText() != Word) { continue; }
                Collision = true;
                break;
            }
            if (Collision) { continue; }
            
            return Word;
        }
        
        return string();
    }());
    
    if (Title.empty()) {
        // NB(xenosoz, 2016): Failed to find a proper string. Skipping.
        return;
    }

    TargetTextNodes.push_back([&] {
        TargetTextNode* It = TargetTextNode::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(PointX, GameSize.height - 100.f));

        It->GamePlaying.follow(GamePlaying);
        It->TitleText.update(Title);
        It->ActiveText.follow(ActiveText);
        It->StartPosition.update(It->getPosition());
        It->StopPosition.update(Point(PointX, PointY));
        It->StartPhase.update(random(0.f, (float)M_PI * 2.f));
        It->StopPhase.update((M_PI * 2.f * random(1, 3)) + (M_PI_2 * 3.f));
        It->SwingRatio.update(random(0.02f, 0.06f));
        It->Duration.follow(TargetFallingDuration);
        
        It->OnMatched = [this, It](TargetTextNode&) {
            // NB(xenosoz, 2016): It's hard to determine all target nodes are *wrong* here.
            //   So just add some cosmetic effects here, let refreshActiveTextForSanity(...) do the heavy lifting.

            if (TheRocketNode) {
                TheRocketNode->OnActionDidFinish = [this](RocketNode&) {
                    if (!TheRocketNode) { return; }
                    TheRocketNode->BasePosition.update();
                };
                Point TP = (It->getPosition() +
                            Vec2(0.f, TargetDummyNode::defaultSize().height / 2.f));
                TheRocketNode->TargetPosition.update(TP);
            }

            appendNewTargetDummyNode(It->getPosition(), It->TitleText());
        };
        
        It->OnTimeOver = [this, It](TargetTextNode&) {
            StarFallDepot().soundForTargetDeath().play();

            TargetTextNodes.remove(It);
            refreshActiveTextForSanity();
            MissCount.update(MissCount() + 1);

            // NB(xenosoz, 2016): The remove process always should be the last.
            It->removeFromParent();
        };
        
        TheGameNode->addChild(It);
        return It;
    }());

    BirthCount.update(BirthCount() + 1);
}

void StarFallScene::appendNewTargetDummyNode(Point Position, const string& TitleText) {
    TargetDummyNodes.push_back([&] {
        auto It = TargetDummyNode::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Position);
        
        It->TitleText.update(TitleText);
        It->OnActionDidFinish = [this, It](TargetDummyNode&) {
            TargetDummyNodes.remove(It);

            // NB(xenosoz, 2016): The remove process always should be the last.
            It->removeFromParent();
        };

        TheGameNode->addChild(It);
        return It;
    }());
}

void StarFallScene::refreshActiveTextForSanity() {
    StarFallDepot Depot;
    TargetTextNode* ExactHitNode = nullptr;
    bool Promising = false;

    for (TargetTextNode* TextNode : TargetTextNodes) {
        // NB(xenosoz, 2016): Check if ActiveText is a prefix of TitleText.
        if (TextNode->TitleText().compare(0, ActiveText().length(), ActiveText()) == 0) {
            Promising = true;
            /* No break */
        }
        if (TextNode->TitleText() == ActiveText()) {
            ExactHitNode = TextNode;
            break;
        }
    }
    
    if (ExactHitNode) {
        StarFallDepot().soundForKeyInput().play();
        
        // NB(xenosoz, 2016): Try word sound first, and then hit
        SoundEffect SE = Depot.soundForWord(ExactHitNode->TitleText());
        SE = SE || Depot.soundForTextHit();
        
        SE.play();
        HitCount.update(HitCount() + 1);
        
        ActiveText.update("");
        
        TargetTextNodes.remove(ExactHitNode);
        ExactHitNode->removeFromParent();
        return;
    }
    
    if (Promising) {
        StarFallDepot().soundForKeyInput().play();
        return;
    }

    // NB(xenosoz, 2016): Clear the input text if it's not promising.
    StarFallDepot().soundForTextMiss().play();

    BadText.update(ActiveText());
    ActiveText.update("");
}

void StarFallScene::onEnter() {
    Super::onEnter();
    scheduleUpdate();
}

void StarFallScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    
    prepareTheWork();
    beginTheWork();
}

void StarFallScene::onExit() {
    Super::onExit();
    unscheduleUpdate();
}

void StarFallScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

void StarFallScene::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);
    if (!GamePlaying()) { return; }
    
    GameClock.update(GameClock() + DeltaSeconds);
    WordRegenClock.update(WordRegenClock() + DeltaSeconds);
}

END_NS_STARFALL
