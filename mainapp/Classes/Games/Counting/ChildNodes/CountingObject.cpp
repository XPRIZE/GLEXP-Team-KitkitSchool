//
//  CountingObject.cpp -- Various counting object for Counting game
//  TodoSchool on Nov 4, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CountingObject.hpp"
#include <Common/Basic/CommentStream.h>
#include "Common/Basic/NodeScopeGuard.h"
#include <cocos/ui/UIButton.h>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace todoschool;


CountingObject::CountingObject() {
}

bool CountingObject::init() {
    string skin = "Counting/stone_01.png";
    int weight = 1;

    return init(skin, weight);
}

bool CountingObject::init(const string& imageName, int weight)
{
    if (!Super::init()) { return false; }
    
    _weight = weight;
    
    [&] {
        auto it = Button::create(imageName);
        auto it_size = it->getContentSize();
        
        it->addTouchEventListener([this](Ref *sender, Widget::TouchEventType event) {
            auto guard = NodeScopeGuard(this);
            
            if (onTouchEvent)
                onTouchEvent(event);
        });

        it->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        it->setPosition(it_size / 2.f);
        it->setCascadeOpacityEnabled(true);
        this->setCascadeOpacityEnabled(true);

        setContentSize(it_size);
        addChild(it);
        return it;
    }();
    
    return true;
}

int CountingObject::weight() const {
    return _weight;
}

int CountingObject::value() const {
    return _value;
}

void CountingObject::setValue(int value) {
    _value = value;
}
