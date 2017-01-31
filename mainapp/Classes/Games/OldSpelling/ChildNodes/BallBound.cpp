//
//  BallBound.cpp -- Boundary for SpellBall
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "BallBound.h"
#include "../Utils/SpellingDepot.h"


BEGIN_NS_SPELLING;

namespace {
    Size contentSize() { return SpellingDepot().gameSize(); }
}  // unnamed namespace

bool BallBound::init() {
    if (!Super::init()) { return false; }

    clearInternals();
    refreshChildNodes();
    refreshChildComponents();

    return true;
}

void BallBound::clearInternals() {
    setContentSize(contentSize());
}

void BallBound::refreshChildNodes() {
    
}

void BallBound::refreshChildComponents() {
    removeAllComponents();
    Size CS = getContentSize();
    
    [&] {
        // NB(xenosoz, 2016): Attach a physics body
        float Thickness = 100.f;
        Size BoxSize(CS.width + Thickness * 2, CS.height + Thickness * 2);

        auto It = PhysicsBody::createEdgeBox(BoxSize,
                                             PhysicsMaterial(.1f, 0.f, .5f),
                                             Thickness);
        It->setDynamic(false);

        setPhysicsBody(It);
        return It;
    }();
}

END_NS_SPELLING;
