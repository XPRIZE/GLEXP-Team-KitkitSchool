//
//  TrailerNode.cpp
//  TodoSchool on Jul 1, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TrailerNode.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_LETTERTRACE

TrailerNode::TrailerNode()
: NextLabel(nullptr)
, NextButton(nullptr)
{
}

bool TrailerNode::init() {
    if (!Super::init()) { return false; }
    
    clear();
    return true;
}

void TrailerNode::clear() {
    clearInternals();

    updateNextLabel();
    updateNextButton();
}

void TrailerNode::clearInternals() {
    using std::placeholders::_1;
    NextButtonText.OnValueUpdate = bind(&TrailerNode::handleNextButtonTextValueUpdate, this, _1);
}

void TrailerNode::updateNextLabel() {
    MainDepot Depot;
    Size GameSize = Depot.gameSize();
    
    if (NextLabel)
        NextLabel->removeFromParent();
    
    NextLabel = ([&] {
        Label* It = Depot.createNextLabel();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width/2.f, GameSize.height - 258.f));
        
        addChild(It);
        return It;
    }());
}

void TrailerNode::updateNextButton() {
    MainDepot Depot;
    Size GameSize = Depot.gameSize();

    if (NextButton)
        NextButton->removeFromParent();

    NextButton = ([&] {
        Button* It = Depot.createNextButton(NextButtonText());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(GameSize.width/2.f, GameSize.height - 684.f));
        It->addClickEventListener([this](Ref*) {
            auto Guard = NodeScopeGuard(this);

            if (OnNextButtonClicked)
                OnNextButtonClicked(this);
        });
        
        addChild(It);
        return It;
    }());
}

void TrailerNode::handleNextButtonTextValueUpdate(std::string &) {
    updateNextButton();
}

END_NS_LETTERTRACE