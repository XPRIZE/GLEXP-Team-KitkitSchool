//
//  RedDot.cpp -- A red dot
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "RedDot.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    string contentSkin() {
        return MainDepot().assetPrefix() + "/ChildNodes/Digital-Quiz_image_dot.png";
    }
}  // unnamed namespace

RedDot::RedDot() {
}

bool RedDot::init() {
    if (!Super::init()) { return false; }
 
    clearInternals();
    refreshChildNodes();
    return true;
}

void RedDot::clearInternals() {
    setCascadeOpacityEnabled(true);
}

void RedDot::refreshChildNodes() {
    removeAllChildren();
    
    [&] {
        auto It = Sprite::create(contentSkin());
        if (!It) { return (It = nullptr); }
        
        auto CS = It->getContentSize();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        setContentSize(CS);
        addChild(It);
        return It;
    }();
}

END_NS_DIGITALQUIZ
