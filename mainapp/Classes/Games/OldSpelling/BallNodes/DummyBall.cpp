//
//  DummyBall.cpp -- A ball with a spell on it
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "DummyBall.h"
#include "../Utils/SpellingDepot.h"


BEGIN_NS_SPELLING;

bool DummyBall::init() {
    if (!Super::init()) { return false; }
    
    return true;
}

void DummyBall::clearInternals() {
    Super::clearInternals();
}

void DummyBall::refreshChildNodes() {
    Super::refreshChildNodes();

}

void DummyBall::refreshChildComponents() {
    Super::refreshChildComponents();
}

END_NS_SPELLING;