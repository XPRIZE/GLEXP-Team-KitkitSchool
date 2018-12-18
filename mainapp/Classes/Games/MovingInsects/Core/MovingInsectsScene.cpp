//
//  MovingInsectsScene.cpp on Sep 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MovingInsectsScene.h"
#include "../Utils/MovingInsectsMainDepot.h"

#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Managers/StrictLogManager.h>

#include "Common/Effects/FireworksEffect.hpp"
#include "Managers/GameSoundManager.h"
#include "CCAppController.hpp"

BEGIN_NS_MOVINGINSECTS

namespace {
    string contentSkin() { return MainDepot().assetPrefix() + "/Background/moving_image_bg.png"; }
    float durationForCardPause() { return .0f; }

    size_t numberOfChoices() { return 10; }
}  // unnamed namespace


MovingInsectsScene* MovingInsectsScene::createScene(int LevelID) {
    return MovingInsectsScene::create();
}

MovingInsectsScene::MovingInsectsScene()
: WorkID(1)
, TheGameNode(nullptr)
, TheProgressBar(nullptr)
, TheGameBoard(nullptr)
, TheDeckBase(nullptr)
, ThePlayerBase(nullptr)
, TheCountField(nullptr)
{
}

bool MovingInsectsScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void MovingInsectsScene::clearInternals() {
    TheWork.OnValueUpdate = [this](MainWork&) {
        refreshChildNodes();
    };
}

void MovingInsectsScene::refreshChildNodes() {
    removeAllChildren();
    AnswerCards.clear();
    
    MainDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    if (true) {
        // NB(xenosoz, 2016): The background node.
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
        It->setCurrent((int)WorkID);

        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): The back button.
        auto It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
            
            if (OnFail)
                OnFail();
        });
        
        addChild(It);
        return It;
    }();


    // NB(xenosoz, 2016): Direct descendants of TheGameNode
    
    TheGameBoard = ([&] {
        auto It = GameBoard::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 36.f + Card::defaultSize().height + 50.f));
        
        TheGameNode->addChild(It);
        return It;
    }());
    
    TheDeckBase = ([&] {
        // NB(xenosoz, 2016): The deck base at the out of the screen.
        auto It = DeckBase::create();
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(GameSize.width * 1.10f, 77.f));
        
        TheGameNode->addChild(It);
        return It;
    }());
    
    ThePlayerBase = ([&] {
        // NB(xenosoz, 2016): The player base.
        auto It = PlayerBase::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(GameSize.width / 2.f, 36.f));
        
        It->SlotCount.update(numberOfChoices());
        
        TheGameNode->addChild(It);
        return It;
    }());
    
    TheCountField = ([&] {
        auto It = CountField::create();
        float TopY = GameSize.height;
        float BottomY = 36.f + Card::defaultSize().height + 50.f + GameBoard::defaultSize().height;
        It->setContentSize(Size(GameSize.width, TopY - BottomY));
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width / 2.f, GameSize.height));
        
        TheGameNode->addChild(It);
        return It;
    }());

    size_t CorrectCardID = (size_t)TheWork().Answer;
    for (size_t CardID = 0; CardID < numberOfChoices(); ++CardID) {
        // NB(xenosoz, 2016): A card.
        auto It = Card::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        It->setToFront();
        
        It->TouchEnabled.update(true);
        It->TitleText.update(itos((int)CardID));
        
        It->OnFrontFaceTouchDidBegin = [this, It](Card&) {
            lockTouchEventOnlyForTheCard(It);
        };
        It->OnFrontFaceTouchDidCancel = [this](Card&) {
            releaseTouchEventForAllCards();
        };
        It->OnFrontFaceTouchDidEnd = [this](Card&) {
            // NB(xenosoz, 2016): Do nothing.
        };
        
        It->OnFrontFaceClicked = [this, It, CardID, CorrectCardID](Card&) {
            if (!TheGameBoard) { return; }
            
            // NB(xenosoz, 2018): Log for future analysis
            auto workPath = [this] {
                stringstream SS;
                SS << "/" << "MovingInsects";
                SS << "/" << "level-" << LevelID << "-" << SheetID;
                SS << "/" << "work-" << WorkID;
                return SS.str();
            }();
            
            StrictLogManager::shared()->game_Peek_Answer("MovingInsects", workPath,
                                                         TodoUtil::itos(CardID),
                                                         TodoUtil::itos(CorrectCardID));

            lockTouchEventForAllCards();
            
            if (CardID == CorrectCardID) {
                // NB(xenosoz, 2016): We hit the correct answer
                Vector<FiniteTimeAction*> Actions;
                Actions.pushBack(TheGameBoard->actionForPullCardToAnswerSlot(It));
                Actions.pushBack(DelayTime::create(.9f));
                Actions.pushBack(CallFunc::create([this] {
                    FireworksEffect::fire();
                    int random = RandomHelper::random_int(1, 5);
                    GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/" + StringUtils::format("card_hit_%d.m4a", random));
                }));
                Actions.pushBack(DelayTime::create(1.0f));
                Actions.pushBack(CallFunc::create([this] {
                    handleCorrectAnswer();
                }));

                Actions.pushBack(DelayTime::create(.1f));
                Actions.pushBack(CallFunc::create([this] {
//                    MainDepot().soundForCardHit().play();
                    if (TheProgressBar)
                        TheProgressBar->setCurrent((int)WorkID, true);
                }));

                It->stopAllActions();
                It->runAction(It->movementGuard(Sequence::create(Actions)));
                return;
            }
            
            // NB(xenosoz, 2016): We hit the wrong answer
            if (!ThePlayerBase) { return; }
            
            Vector<FiniteTimeAction*> Actions;
            Actions.pushBack(TheGameBoard->actionForPullCardToAnswerSlot(It));
            Actions.pushBack(CallFunc::create([this] {
                MainDepot().soundForCardMiss().play();
                FireworksEffect::miss();
            }));
            
            Actions.pushBack(ThePlayerBase->actionForPullCardWithSlotIndex(It, CardID));
            Actions.pushBack(ThePlayerBase->actionForShakeCardWithSlotIndex(It, CardID));
            Actions.pushBack(DelayTime::create(durationForCardPause()));
            Actions.pushBack(CallFunc::create([this] {
                releaseTouchEventForAllCards();
            }));
  
            It->stopAllActions();
            It->runAction(It->movementGuard(Sequence::create(Actions)));
            
            if (TheCountField)
                TheCountField->shakeGroups(.2f);

        };
        
        It->setVisible(false);
        TheGameNode->addChild(It);
        AnswerCards.push_back(It);
        It->setToBack();
        TheDeckBase->catchCard(It);
        It->setVisible(true);
        
        // NB(xenosoz, 2016): Cards appear
        Vector<FiniteTimeAction*> Actions;
        auto AppearKey = MainDepot().keyForCardAppear();
        auto AppearTime = AppearKey.interpolate((float)CardID / (numberOfChoices() + 1));

        Actions.pushBack(DelayTime::create(AppearTime));
        Actions.pushBack(ThePlayerBase->actionForPullCardWithSlotIndex(It, CardID));
        Actions.pushBack(CallFunc::create([this] {
            // NB(xenosoz, 2016): Put card birth sound here.
        }));
        Actions.pushBack(DelayTime::create(AppearKey.endTime() - AppearTime));

        // NB(xenosoz, 2016): Cards flip to front
        auto OpenKey = MainDepot().keyForCardOpen();
        auto OpenTime = OpenKey.interpolate((float)CardID / (numberOfChoices() + 1));

        Actions.pushBack(DelayTime::create(OpenTime - AppearKey.endTime()));
        Actions.pushBack(It->actionForFlipToFront());
        Actions.pushBack(CallFunc::create([this] {
            // NB(xenosoz, 2016): Put card flip sound here.
        }));
        Actions.pushBack(DelayTime::create(OpenKey.endTime() - OpenTime));
        
        It->stopAllActions();
        It->runAction(It->movementGuard(Sequence::create(Actions)));
    }
}

void MovingInsectsScene::lockTouchEventOnlyForTheCard(Card* ActiveCard) {
    for (auto C : AnswerCards) {
        if (C != ActiveCard) C->TouchEnabled.update(false);
    }
}

void MovingInsectsScene::lockTouchEventForAllCards() {
    for (auto C : AnswerCards)
        C->TouchEnabled.update(false);
}

void MovingInsectsScene::releaseTouchEventForAllCards() {
    for (auto C : AnswerCards) {
        C->TouchEnabled.update(true);
        C->TouchEventEndedPrematually = false;
    }
}


void MovingInsectsScene::prepareFirstWork() {
    WorkID = 1;

    auto Work = TheSheet->workByID(WorkID);
    TheWork.update(Work);
}

bool MovingInsectsScene::prepareNextWork() {
    WorkID += 1;

    if (WorkID >= TheSheet->endWorkID()) {
        return false;
    }

    auto Work = TheSheet->workByID(WorkID);
    TheWork.update(Work);

    return true;
}

void MovingInsectsScene::beginTheWork() {
    if (!TheCountField || !TheGameBoard) { return; }
    auto Work = TheWork();

    [&] {
        if (Work.Operator == "+") {
            // NB(xenosoz, 2016): Group A for addition
            scheduleOnce([this, Work](float) {
                if (!TheCountField) { return; }
                TheCountField->appendGroupA(Work.LeftExpr, Work.RightExpr,
                                            MainDepot().keyForGroupA().interval());
            }, MainDepot().keyForGroupA().beginTime(), "MovingInsectsScene::onEnter::appendGroupA");

            // NB(xenosoz, 2016): Group B for addition
            scheduleOnce([this, Work](float) {
                if (!TheCountField) { return; }
                TheCountField->appendGroupB(Work.LeftExpr, Work.RightExpr,
                                            MainDepot().keyForGroupB().interval());
            }, MainDepot().keyForGroupB().beginTime(), "MovingInsectsScene::onEnter::appendGroupB");

            return;
        }

        if (Work.Operator == "-") {
            // NB(xenosoz, 2016): Group A for subtraction
            scheduleOnce([this, Work](float) {
                if (!TheCountField) { return; }
                TheCountField->appendGroups(Work.LeftExpr - Work.RightExpr, Work.RightExpr,
                                            MainDepot().keyForGroupA().interval());
            }, MainDepot().keyForGroupA().beginTime(), "MovingInsectsScene::onEnter::appendGroups");
            
            // NB(xenosoz, 2016): Group B for subtraction
            scheduleOnce([this, Work](float) {
                if (!TheCountField) { return; }
                TheCountField->removeGroupB(MainDepot().keyForGroupB().interval());
            }, MainDepot().keyForGroupB().beginTime(), "MovingInsectsScene::onEnter::removeGroupB");

            return;
        }
        
        CCLOGERROR("Work.Operator has unexpected value in MovingInsectsScene::prepareCurrentWork: %s", Work.Operator.c_str());
    }();
    
    
    // NB(xenosoz, 2016): Proglem on the game board

    TheGameBoard->LeftExpr.update(itos(Work.LeftExpr));
    TheGameBoard->Operator.update(Work.Operator);
    TheGameBoard->RightExpr.update(itos(Work.RightExpr));
    
    scheduleOnce([this, Work](float) {
        if (!TheGameBoard) { return; }
        
        TheGameBoard->showLeftExpr();
        MainDepot().soundForCardinalNumber(Work.LeftExpr).play();
    }, MainDepot().keyForLeftExpr().beginTime(), "MovingInsectsScene::onEnter::showLeftExpr");
    
    scheduleOnce([this, Work](float) {
        if (!TheGameBoard) { return; }
        
        TheGameBoard->showOperator();
        MainDepot().soundForOperator(Work.Operator).play();
    }, MainDepot().keyForOperator().beginTime(), "MovingInsectsScene::onEnter::showOperator");
    
    scheduleOnce([this, Work](float) {
        if (!TheGameBoard) { return; }
        
        TheGameBoard->showRightExpr();
        MainDepot().soundForCardinalNumber(Work.RightExpr).play();
    }, MainDepot().keyForRightExpr().beginTime(), "MovingInsectsScene::onEnter::showRightExpr");
    
    scheduleOnce([this, Work](float) {
        if (!TheGameBoard) { return; }
        
        TheGameBoard->showEquals();
        MainDepot().soundForOperator("=").play();
    }, MainDepot().keyForEquals().beginTime(), "MovingInsectsScene::onEnter::showEquals");

    scheduleOnce([this, Work](float) {
        if (!TheGameBoard) { return; }
        
        TheGameBoard->showQuestionMark();
        MainDepot().soundForCardBirth().play();
    }, MainDepot().keyForQuestionMark().beginTime(), "MovingInsectsScene::onEnter::showQuestionMark");

}

void MovingInsectsScene::handleCorrectAnswer() {
    if (prepareNextWork()) {
        beginTheWork();
        return;
    }
    
    handleSuccess();
}

void MovingInsectsScene::handleSuccess() {
    auto CP = CompletePopup::create();
    CP->show(0.f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();
        
        CCAppController::sharedAppController()->handleGameComplete(1);
//        TodoSchoolBackButton::popGameScene();
    });
}

void MovingInsectsScene::onEnterTransitionDidFinish() {
    Super::onEnterTransitionDidFinish();
    
    prepareFirstWork();
    beginTheWork();
}

void MovingInsectsScene::onExitTransitionDidStart() {
    Super::onExitTransitionDidStart();
}


END_NS_MOVINGINSECTS
