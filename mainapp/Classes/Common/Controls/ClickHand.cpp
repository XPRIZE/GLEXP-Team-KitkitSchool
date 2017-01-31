//
//  ClickHand.cpp
//  todomath
//
//  Created by Gunho Lee on 2/15/16.
//
//

#include "ClickHand.hpp"


bool ClickHand::init()
{
    
    if (!Node::init()) {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    
    _handView = Sprite::create("tutorial_image_guideHand_normal.png");
    if (!_handView) return false;
    
    setContentSize(_handView->getContentSize());
    
    _handView->setAnchorPoint(Vec2(0, 0));
    _handView->setPosition(Vec2(0, 0));
    this->addChild(_handView);
    

    this->setAnchorPoint(Vec2(0, 1));
    
    
    return true;
    
}


void ClickHand::startAnimation()
{
    _handView->stopAllActions();
    
    auto seq = Sequence::create(
        CallFunc::create([this](){
            _handView->setTexture("tutorial_image_guideHand_touch.png");
            _handView->setPosition(Vec2(0, 0));
        }),
        EaseOut::create(MoveTo::create(0.35, Vec2(-10, -20)), 2.0),
                                
        CallFunc::create([this](){
        _handView->setTexture("tutorial_image_guideHand_normal.png");
        }),
        EaseOut::create(MoveTo::create(0.35, Vec2(0, 0)), 2.0),
        nullptr);
    
    _handView->runAction(RepeatForever::create(seq));
    
    
}

void ClickHand::stopAnimation()
{
    _handView->stopAllActions();
}

void ClickHand::pressAnimation(bool animate)
{
    _handView->setTexture("tutorial_image_guideHand_touch.png");
    _handView->setPosition(Vec2(0, 0));
    if (animate) {
        _handView->stopAllActions();
        _handView->runAction(EaseOut::create(MoveTo::create(0.35, Vec2(-10, -20)), 2.0));
    }
}

void ClickHand::releaseAnimation(bool animate)
{
    _handView->setTexture("tutorial_image_guideHand_normal.png");
    _handView->setPosition(Vec2(-10, -20));
    if (animate) {
        _handView->stopAllActions();
        _handView->runAction(EaseOut::create(MoveTo::create(0.35, Vec2(0, 0)), 2.0));
    }
    
}
