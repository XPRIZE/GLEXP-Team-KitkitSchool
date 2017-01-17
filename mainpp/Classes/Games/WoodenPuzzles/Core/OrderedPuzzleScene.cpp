//
//  OrderedPuzzleScene.h -- A puzzle game with "complete one piece, get a new piece" rule.
//  TodoSchool on Aug 29, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "OrderedPuzzleScene.h"
#include "../PieceNodes/WoodPiece.h"
#include "../ChildNodes/WoodPieceShadow.h"
#include "../ChildNodes/WoodSlot.h"
#include "../Utils/WoodenPuzzleDepot.h"

#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoSchoolBackButton.hpp>

#include "CCAppController.hpp"

BEGIN_NS_WOODENPUZZLES

namespace {
    bool showDebugLog() { return true; }

    int localZOrderForShadowField() { return -20; }
    int localZOrderForGameBoard() { return -10; }
    int localZOrderForPlayField() { return 0; }

    float delayForRefillPiece() { return .4f; }
    float delayForPieceCreation() { return .4f; }
    
    string contentSkin() {
        return WoodenPuzzleDepot().assetPrefix() + "/Background/wp_background.jpg";
    }
    
    bool pieceShouldPutInSlot(WoodPiece* Piece, WoodSlot* Slot) {
        auto convertToWorldSpace = [](Node* N, Rect R) {
            auto Origin = N->convertToWorldSpace(R.origin);
            auto Diagonal = N->convertToWorldSpace(R.origin + R.size);
            
            return Rect(Origin, Size(Diagonal - Origin));
        };
        
        Rect PieceR = convertToWorldSpace(Piece, Rect(Point::ZERO, Piece->getContentSize()));
        Rect SlotR = convertToWorldSpace(Slot, Rect(Point::ZERO, Slot->getContentSize()));
        Rect Cap = PieceR.intersection(SlotR);
        
        return (Cap.area() >= PieceR.area() * 0.60f ||
                Cap.area() >= SlotR.area() * 0.60f);
    }
}  // unnamed namespace


OrderedPuzzleScene::OrderedPuzzleScene()
: TheGameNode(nullptr)
, TheGameBoard(nullptr)
, ThePlayField(nullptr)
, TheShadowField(nullptr)
, AnswerCount(0)
{
}

bool OrderedPuzzleScene::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void OrderedPuzzleScene::grabPuzzleStore(WoodenPuzzleStore* Store) {
    ThePuzzleStore.reset(Store);
    refreshChildNodes();
}


void OrderedPuzzleScene::clearInternals() {
    WoodenPuzzleDepot().preloadSoundEffects();
}

void OrderedPuzzleScene::refreshChildNodes() {
    removeAllChildren();
    
    WoodenPuzzleDepot Depot;
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
    
    if (true) {
        // NB(xenosoz, 2016): The back button.
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        It->addClickEventListener([this](Ref*) {
            handleFail();
        });
        
        addChild(It);
    }
    
    // NB(xenosoz, 2016): Descendants of the game node.
    if (!ThePuzzleStore) { return; }
    auto& Store = *ThePuzzleStore;
    
    TheGameBoard = ([&] {
        auto It = Store.createGameBoard();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        It->setPosition(Point(100.f, GameSize.height / 2.f));
        
        TheGameNode->addChild(It, localZOrderForGameBoard());
        return It;
    }());
    
    TheDeckBase = ([&] {
        auto It = DeckBase::create();
        auto ItCS = It->getContentSize();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
        It->setPosition(Point(GameSize.width - 100.f, GameSize.height / 2.f));
        
        It->feedPuzzleStore(ThePuzzleStore);

        TheGameNode->addChild(It);
        return It;
    }());

    ThePlayField = ([&] {
        auto It = Store.createEmptyPlayField();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        
        TheGameNode->addChild(It, localZOrderForPlayField());

        fillWoodPiecesToMaxCount(It);
        return It;
    }());
    
    TheShadowField = ([&] {
        // NB(xenosoz, 2016): The shadow field.
        auto It = ShadowField::create();
        It->setContentSize(GameSize);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        
        if (showDebugLog()) {
            REPR(It->getContentSize());
            REPR(It->getPosition());
        }
        
        if (ThePlayField) {
            for (auto Item : ThePlayField->WoodPieces) {
                WoodPiece* P = Item.second;
                auto S = WoodPieceShadow::create();
                
                S->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                S->setPosition(P->getPosition());
                
                S->PieceID.follow(P->PieceID);
                S->PositionInWorldSpace.follow(P->PositionInWorldSpace);
                
                It->appendShadowForPiece(S, P);
            }
        }
        
        TheGameNode->addChild(It, localZOrderForShadowField());
        return It;
    }());
}

void OrderedPuzzleScene::fillWoodPiecesToMaxCount(PlayField* Field) {
    if (!Field || !TheDeckBase) { return; }
    
    TheDeckBase->pullPiecesToHomePosition();

    while (true) {
        auto Index = TheDeckBase->size();
        auto P = TheDeckBase->createNextWoodPiece();
        if (!P) { break; }
        
        Point Position = Field->convertToNodeSpace(TheDeckBase->positionInWorldSpaceForIndex(Index));
        P->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        P->setPosition(Position);
        P->setVisible(false);
        
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(delayForPieceCreation()));
        Actions.pushBack(Show::create());
        Actions.pushBack(CallFunc::create([] {
            WoodenPuzzleDepot().soundForCardBirth().play();
        }));

        auto Action = Sequence::create(Actions);
        Action->setTag(WoodenPuzzleDepot().actionTagForPieceCreation());
        P->stopActionByTag(Action->getTag());
        P->runAction(Action);

        attachGameLogicToWoodPiece(P);

        Field->appendWoodPiece(P);
        TheDeckBase->appendWoodPiece(P);
    }
}

void OrderedPuzzleScene::refillWoodPiecesToMaxCount() {
    if (!ThePlayField) { return; }

    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For refilling wood pieces
        fillWoodPiecesToMaxCount(ThePlayField);
    }, delayForRefillPiece(), "OrderedPuzzleScene::refillWoodPiecesToMaxCount::delayRefillPiece");
}

void OrderedPuzzleScene::attachGameLogicToWoodPiece(WoodPiece* Piece) {
    if (!Piece) { return; }
    
    Piece->OnTouch = [this, Piece](Touch*, Event*) {
        auto Field = ThePlayField;
        if (!Field) { return; }
        
        if (Piece->PieceNameSound)
            Piece->PieceNameSound.play();
        Field->renice(Piece);
    };
    
    Piece->OnDrag = [this, Piece](Touch*, Event*) {
        auto Board = TheGameBoard;
        auto Field = ThePlayField;
        auto Shadows = TheShadowField;
        if (!Board || !Field || !Shadows || !TheDeckBase) { return; }
        
        auto SlotIt = Board->WoodSlots.find(Piece->PieceID());
        if (SlotIt == Board->WoodSlots.end()) { return; }
        WoodSlot* Slot = SlotIt->second;
        
        if (pieceShouldPutInSlot(Piece, Slot)) {
            auto PParent = Piece->getParent();
            if (PParent) {
                Point P = PParent->convertToWorldSpace(Piece->getPosition());

                Board->promoteWoodSlot(Slot);
                Slot->pullPieceIllusion(P);
            }
            else {
                Slot->fillSlot();
            }
            Field->removeWoodPiece(Piece);
            Shadows->removeShadowForPiece(Piece);
            
            TheDeckBase->removeWoodPiece(Piece);
            refillWoodPiecesToMaxCount();
            
            auto Guard = NodeScopeGuard(this);
            handleCorrectAnswer();
        }
    };
    
    Piece->OnRelease = [this](Touch*, Event*) {
        if (!TheDeckBase) { return; }

        TheDeckBase->pullPiecesToHomePosition();
        WoodenPuzzleDepot().soundForCardMiss().play();
    };
}

void OrderedPuzzleScene::handleCorrectAnswer() {
    AnswerCount += 1;
    if (AnswerCount >= ThePuzzleStore->motifs().size()) {
        handleSuccess();
    }
}

void OrderedPuzzleScene::handleSuccess() {
    auto CP = CompletePopup::create();
    CP->show(0.8f, [this] {
        auto Guard = NodeScopeGuard(this);
        
        if (OnSuccess)
            OnSuccess();

        CCAppController::sharedAppController()->handleGameComplete(1);
//        TodoSchoolBackButton::popGameScene();
    });
}

void OrderedPuzzleScene::handleFail() {
    auto Guard = NodeScopeGuard(this);

    if (OnFail)
        OnFail();
}

END_NS_WOODENPUZZLES
