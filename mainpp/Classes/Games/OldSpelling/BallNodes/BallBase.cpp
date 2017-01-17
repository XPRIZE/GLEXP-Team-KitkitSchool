//
//  BallBase.cpp -- A ball with a spell on it
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "BallBase.h"
#include "../Utils/SpellingDepot.h"


BEGIN_NS_SPELLING;


namespace {
    string contentSkin() {
        int BallID = 6;  // XXX
        return SpellingDepot().assetPrefix() + format("/Temp/ball_%d.png", BallID);
    }
    string defaultFont() { return SpellingDepot().defaultFont(); }
    float defaultFontSize() { return 200.f; }
}  // unnamed namespace


bool BallBase::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    refreshChildComponents();
    
    return true;
}

void BallBase::clearInternals() {
    BallClass.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void BallBase::refreshChildNodes() {
    removeAllChildren();
    
    [&] {
        // NB(xenosoz, 2016): The sprite
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(It->getContentSize() / 2.f);
        
        setContentSize(It->getContentSize());
        addChild(It);
        return It;
    }();

    
    Size CS = getContentSize();
    
    [&] {
        // NB(xenosoz, 2016): The label
        auto It = Label::createWithTTF(BallClass(), defaultFont(), defaultFontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
        return It;
    }();
}

void BallBase::refreshChildComponents() {
}

END_NS_SPELLING;