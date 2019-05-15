//
//  PlayerBase.cpp on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FindTheMatchPlayerBase.h"
#include "../Utils/FindTheMatchMainDepot.h"

BEGIN_NS_FINDTHEMATCH

namespace {
    Size contentSize() {
        MainDepot Depot;
        return Size(Depot.gameSize().width, Depot.cardSize().height);
    }
    float durationForCardMove() {
        return .12f;
    }
}  // unnamed namespace


PlayerBase::PlayerBase() {
}

bool PlayerBase::init() {
    if (!Super::init()) { return false; }

    clearInternals();

    return true;
}

FiniteTimeAction* PlayerBase::actionForPullCardWithSlotIndex(Card* C, size_t Index) {
    Point P;
    if (!pointInCardParent(P, C, Index)) {
        return CallFunc::create([]{});
    }

    return MoveTo::create(durationForCardMove(), P);
}

FiniteTimeAction* PlayerBase::actionForShakeCardWithSlotIndex(Card* C, size_t Index) {
    Point P;
    if (!pointInCardParent(P, C, Index)) {
        CallFunc::create([]{});
    }
    
    return C->actionForShake(P);
}

void PlayerBase::pullCardWithSlotIndex(Card* C, size_t Index) {
    if (!C || !C->getParent()) { return; }
    if (SlotCount() <= 0) { return; }
    
    C->stopAllActions();
    C->runAction(C->movementGuard(actionForPullCardWithSlotIndex(C, Index)));
}

void PlayerBase::clearInternals() {
    setContentSize(contentSize());
}

bool PlayerBase::pointInCardParent(Point& P, Card* C, size_t Index) {
    if (!C || !C->getParent()) { return false; }
    if (SlotCount() <= 0) { return false; }

    MainDepot Depot;
    Size CardSize = Depot.cardSize();
    Size CS = getContentSize();

    float MarginX = 92.f;
    float OffsetX = (CardSize.width + MarginX) * (Index - (SlotCount() - 1) / 2.f);
    
    Point PointInHere(CS.width/2.f + OffsetX, CS.height/2.f);
    Point PointInWorld = convertToWorldSpace(PointInHere);
    Point PointInCardParent = C->getParent()->convertToNodeSpace(PointInWorld);
    
    P = PointInCardParent;
    return true;
}

END_NS_FINDTHEMATCH
