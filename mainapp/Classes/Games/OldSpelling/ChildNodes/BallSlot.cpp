//
//  BallSlot.cpp -- Empty/full slot for SpellBall
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "BallSlot.h"
#include "../Utils/SpellingDepot.h"


BEGIN_NS_SPELLING;


namespace {
    Size contentSize() { return Size(288, 288.f); }
    string contentSkin() { return SpellingDepot().assetPrefix() + "/Temp/empty_slot.png"; }
}  // unnamed namespace


BallSlot::BallSlot()
    : TheDummyBall(nullptr)
{
}

bool BallSlot::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

bool BallSlot::filled() {
    return Filled();
}

void BallSlot::clearSlot() {
    if (!TheDummyBall) { return; }
    
    Filled.update(false);
}

void BallSlot::fillSlot() {
    if (!TheDummyBall) { return; }
    
    Filled.update(true);
}


Size BallSlot::defaultSize() {
    return contentSize();
}


void BallSlot::clearInternals() {
    setContentSize(contentSize());
    
    Filled.update(false);
    Filled.OnValueUpdate = [this](bool&) {
        if (!TheDummyBall) { return; }
        TheDummyBall->setVisible(Filled());
    };

    SlotClass.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void BallSlot::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();
    
    [&] {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS / 2.f));
        
        addChild(It);
        return It;
    }();
    
    TheDummyBall = [&] {
        auto It = DummyBall::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS / 2.f));
        
        It->BallClass.follow(SlotClass);
        It->setVisible(Filled());

        addChild(It);
        return It;
    }();
}

END_NS_SPELLING;
