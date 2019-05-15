//
//  DeckBase.cpp on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MovingInsectsDeckBase.h"
#include "../Utils/MovingInsectsMainDepot.h"


BEGIN_NS_MOVINGINSECTS

namespace {
    Size contentSize() { return MainDepot().cardSize(); }
}  // unnamed namespace


DeckBase::DeckBase() {
}

bool DeckBase::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    
    return true;
}

void DeckBase::catchCard(Card* C) {
    if (!C || !C->getParent()) { return; }
    
    Size CS = getContentSize();
    Point PointInHere = Point(CS) / 2.f;
    Point PointInWorld = convertToWorldSpace(PointInHere);
    Point PointInCardParent = C->getParent()->convertToNodeSpace(PointInWorld);

    C->setPosition(PointInCardParent);
}

void DeckBase::clearInternals() {
    setContentSize(contentSize());
}

END_NS_MOVINGINSECTS
