//
//  FindTheMatchScene.cpp on Aug 2, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FindTheMatchScene.h"
#include "../Utils/MainDepot.h"
#include <Common/Basic/SoundEffect.h>
#include "Common/Effects/FireworksEffect.hpp"

#include "CCAppController.hpp"

BEGIN_NS_FINDTHEMATCH


namespace {
    string contentSkin() {
        return MainDepot().assetPrefix() + "/Background/picturematching.jpg";
    }
    string flowerLeft() {
        return MainDepot().assetPrefix() + "/Background/flowers_left.png";
    }
    string flowerRight() {
        return MainDepot().assetPrefix() + "/Background/flowers_right.png";
    }
    float durationForCardPause() {
        return .05f;
    }
}  // unnamed namespace


FindTheMatchScene* FindTheMatchScene::createScene() {
    return FindTheMatchScene::create();
}

FindTheMatchScene::FindTheMatchScene()
{
}

bool FindTheMatchScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void FindTheMatchScene::clearInternals() {
    MainDepot Depot;
    Depot.preloadSoundEffects();

    TouchEnabled.update(false);

    TheWork.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
    };
}

void FindTheMatchScene::refreshChildNodes() {
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    removeAllChildren();
    AnswerCards.clear();
    
    
    // NB(xenosoz, 2016): Direct descendants of this.
    
    if (true) {
        auto BN = Node::create();
        BN->setContentSize(WindowSize);
        
        if (true) {
            auto It = Sprite::create(contentSkin());
            Size SpriteSize = It->getContentSize();
            float Scale = Depot.scaleToCoverWindow(SpriteSize);

            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(WindowSize/2.f);

            It->setScale(Scale);
            It->setBlendFunc(BlendFunc::DISABLE);
            BN->addChild(It);
        }
        if (true) {
            auto It = Sprite::create(flowerLeft());
            It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            It->setPosition(Point(0.f, WindowSize.height));
            BN->addChild(It);
        }
        if (true) {
            auto It = Sprite::create(flowerRight());
            It->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            It->setPosition(Point(WindowSize));
            BN->addChild(It);
        }
        
        addChild(BN);
    }

    TheGameNode = ([&] {
        Size WindowSize = Depot.windowSize();
        Size GameSize = Depot.gameSize();
        float Scale = Depot.scaleToBeContainedInWindow(GameSize);
        
        Node* It = Node::create();
        It->setContentSize(GameSize);
        It->setScale(Scale);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(WindowSize.width/2.f, 0.f));
        
        addChild(It);
        return It;
    }());
    
    TheProgressBar = [&] {
        // NB(xenosoz, 2016): The progress indicator.
        auto It = ProgressIndicator::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width / 2.f, WindowSize.height - It->getContentSize().height));
        
        It->setMax((int)TheSheet().size());
        It->setCurrent((int)WorkID - TheSheet().beginProblemID() + 1);
        
        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): The back button.
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            NodeScopeGuard Guard(this);
            handleFail();
        });
        
        addChild(It);
        return It;
    }();
    

    // NB(xenosoz, 2016): Direct descendants of the game node.

    TheGameBoard = ([&] {
        auto It = GameBoard::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width / 2.f, GameSize.height - 88.f));
        
        TheGameNode->addChild(It);
        return It;
    }());

    TheDeckBase = ([&] {
        // NB(xenosoz, 2016): The deck base at the out of the screen.
        auto It = DeckBase::create();
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(GameSize.width, 77.f));
        
        TheGameNode->addChild(It);
        return It;
    }());
    
    ThePlayerBase = ([&] {
        // NB(xenosoz, 2016): The player base.
        auto It = PlayerBase::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 77.f));
        
        It->SlotCount.update(TheWork().NumberOfChoices);
        
        TheGameNode->addChild(It);
        return It;
    }());


    size_t CorrectCardID = TheWork().AnswerID;

    if (true) {
        // NB(xenosoz, 2016): The problem card.
        auto It = Card::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        It->setToFront();
        
        // NB(xenosoz, 2016): Let problem card always disabled for touch event.
        It->TouchEnabled.update(false);
        It->TitleAsset.update(TheWork().assetForProblemCard());
        //It->TitleText.update(itos((int)CorrectCardID));

        It->setVisible(false);
        TheGameNode->addChild(It);
        TheGameBoard->catchCardToProblemSlot(It);
        It->setVisible(true);
    }

    for (size_t CardID = 0; CardID < TheWork().NumberOfChoices; ++CardID) {
        // NB(xenosoz, 2016): A card.
        auto It = Card::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        It->setToFront();
        
        It->TouchEnabled.update(true);
        It->TitleAsset.update(TheWork().assetForChoiceCard(CardID));
        //It->TitleText.update(itos((int)CardID));
        
        It->OnFrontFaceTouchDidBegin = [this, It](Card&) {
            lockTouchEventOnlyForTheCard(It);
        };
        It->OnFrontFaceTouchDidCancel =
        It->OnFrontFaceTouchDidEnd = [this](Card&) {
            releaseTouchEventForAllCards();
        };

        It->OnFrontFaceClicked = [this, It, CardID, CorrectCardID](Card&) {
            if (!TheGameBoard) { return; }
            if (CardID == CorrectCardID) {
                Vector<FiniteTimeAction*> Actions;
                Actions.pushBack(TheGameBoard->actionForPullCardToAnswerSlot(It));

                Actions.pushBack(DelayTime::create(.1f));
                Actions.pushBack(CallFunc::create([this] {
                    FireworksEffect::fire();
                    
                    MainDepot().soundForCardHit().play();
                    if (TheProgressBar)
                        TheProgressBar->setCurrent((int)WorkID - TheSheet().beginProblemID() + 1, true);
                }));

                Actions.pushBack(DelayTime::create(.9f));
                Actions.pushBack(CallFunc::create([this] {
                    NodeScopeGuard Guard(this);
                    handleCorrectAnswer();
                }));

                It->stopAllActions();
                It->runAction(It->movementGuard(Sequence::create(Actions)));
                return;
            }
            
            if (!ThePlayerBase) { return; }
            
            Vector<FiniteTimeAction*> Actions;
            Actions.pushBack(TheGameBoard->actionForPullCardToAnswerSlot(It));
            Actions.pushBack(CallFunc::create([this] {
                MainDepot().soundForCardMiss().play();
                FireworksEffect::miss();
            }));
            Actions.pushBack(DelayTime::create(durationForCardPause()));
            Actions.pushBack(CallFunc::create([this] {
                releaseTouchEventForAllCards();
            }));
            Actions.pushBack(ThePlayerBase->actionForPullCardWithSlotIndex(It, CardID));
            Actions.pushBack(ThePlayerBase->actionForShakeCardWithSlotIndex(It, CardID));

            lockTouchEventOnlyForTheCard(It);
            It->stopAllActions();
            It->runAction(It->movementGuard(Sequence::create(Actions)));
        };
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        AnswerCards.push_back(It);
        It->setToBack();
        TheDeckBase->catchCard(It);
        It->setVisible(true);

        // NB(xenosoz, 2016): Initial movement
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(.2f * CardID));
        Actions.pushBack(ThePlayerBase->actionForPullCardWithSlotIndex(It, CardID));
        Actions.pushBack(CallFunc::create([this] {
            MainDepot().soundForCardBirth().play();
        }));
        Actions.pushBack(DelayTime::create(.2f * (max(4, (int)TheWork().NumberOfChoices) - 1)));
        Actions.pushBack(It->actionForFlipToFront());
        Actions.pushBack(CallFunc::create([this] {
            MainDepot().soundForCardDeath().play();
        }));

        It->stopAllActions();
        It->runAction(It->movementGuard(Sequence::create(Actions)));
    }
    
}

void FindTheMatchScene::lockTouchEventOnlyForTheCard(Card* ActiveCard) {
    for (auto C : AnswerCards) {
        if (C == ActiveCard) { continue; }
        C->TouchEnabled.update(false);
    }
}

void FindTheMatchScene::releaseTouchEventForAllCards() {
    for (auto C : AnswerCards) {
        C->TouchEnabled.update(true);
    }
}

void FindTheMatchScene::prepareFirstWork() {
    WorkID = TheSheet().beginProblemID();

    auto Work = TheSheet().problemByID(WorkID);
    TheWork.update(Work);
}

bool FindTheMatchScene::prepareNextWork() {
    WorkID += 1;
    
    if (WorkID >= TheSheet().endProblemID()) {
        return false;
    }
    
    auto Work = TheSheet().problemByID(WorkID);
    TheWork.update(Work);
    
    return true;
}

void FindTheMatchScene::beginTheWork() {
    // NB(xenosoz, 2016): Consult ``FindTheMatchScene::refreshChildNode(...)''.
    GamePlaying.update(true);
}

void FindTheMatchScene::handleCorrectAnswer() {
    if (prepareNextWork()) {
        beginTheWork();
        return;
    }
    
    handleSuccess();
}

void FindTheMatchScene::handleSuccess() {
    GamePlaying.update(false);
    
    auto CP = CompletePopup::create();
    CP->show(0.f, [this] {
        auto Guard = NodeScopeGuard(this);

        if (OnSuccess)
            OnSuccess();

        CCAppController::sharedAppController()->handleGameComplete(1);
//        TodoSchoolBackButton::popGameScene();
    });
}

void FindTheMatchScene::handleFail() {
    if (OnFail) {
        NodeScopeGuard Guard(this);
        OnFail();
    }
}

void FindTheMatchScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    
    prepareFirstWork();
    beginTheWork();
}

void FindTheMatchScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}

END_NS_FINDTHEMATCH
