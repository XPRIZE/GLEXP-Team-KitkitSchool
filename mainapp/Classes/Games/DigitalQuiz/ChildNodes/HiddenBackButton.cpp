//
//  HiddenBackButton.cpp -- Hidden back button
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "HiddenBackButton.h"
#include "../Utils/DigitalQuizMainDepot.h"


BEGIN_NS_DIGITALQUIZ

namespace {
    string normalSkin() {
        return MainDepot().assetPrefix() + "/ChildNodes/Digital-Quiz_button_back_normal.png";
    }
}  // unnamed namespace


HiddenBackButton::HiddenBackButton()
{
}

bool HiddenBackButton::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void HiddenBackButton::clearInternals() {
    setCascadeOpacityEnabled(true);
}

void HiddenBackButton::refreshChildNodes() {
    removeAllChildren();
    
    [&] {
        auto It = Button::create(normalSkin());
        auto CS = It->getContentSize();
        
        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);
            if (OnClick)
                OnClick();
        });
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        setContentSize(CS);
        addChild(It);
        return It;
    }();
    
    //Size CS = getContentSize();
}

END_NS_DIGITALQUIZ
