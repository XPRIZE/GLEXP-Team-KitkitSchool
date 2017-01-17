//
//  GameBoard.cpp on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "GameBoard.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_FINDTHEMATCH


namespace {
    string contentSkin() {
        return MainDepot().assetPrefix() + "/GameBoard/game_board.png";
    }
    Size contentSize() {
        return Size(1315.f, 904.f);
    }
    float durationForCardMove() {
        return .3f;
    }
}  // unnamed namespace


GameBoard::GameBoard() {
}

bool GameBoard::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void GameBoard::catchCardToProblemSlot(Card* C) {
    if (!C || !C->getParent()) { return; }
    
    Point PointInHere((105.f + 605.f) / 2.f, (146.f + 805.f - 11.f) / 2.f);
    Point PointInWorld = convertToWorldSpace(PointInHere);
    Point PointInCardParent = C->getParent()->convertToNodeSpace(PointInWorld);

    C->stopAllActions();
    C->setPosition(PointInCardParent);
}

FiniteTimeAction* GameBoard::actionForPullCardToAnswerSlot(Card* C) {
    if (!C || !C->getParent()) { return CallFunc::create([]{}); }
    
    Point PointInHere((698.f + 1198.f) / 2.f, (146.f + 805.f - 11.f) / 2.f);
    Point PointInWorld = convertToWorldSpace(PointInHere);
    Point PointInCardParent = C->getParent()->convertToNodeSpace(PointInWorld);
    
    return MoveTo::create(durationForCardMove(), PointInCardParent);
}

void GameBoard::pullCardToAnswerSlot(Card* C) {
    if (!C || !C->getParent()) { return; }
    
    C->stopAllActions();
    C->runAction(C->movementGuard(actionForPullCardToAnswerSlot(C)));
}


void GameBoard::clearInternals() {
    setContentSize(contentSize());
}

void GameBoard::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();
    
    if (true) {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
    }
}


END_NS_FINDTHEMATCH