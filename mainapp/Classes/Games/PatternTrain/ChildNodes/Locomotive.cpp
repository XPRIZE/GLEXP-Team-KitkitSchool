//
//  Locomotive.cpp
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
//

#include "Locomotive.hpp"

BEGIN_NS_PATTERNTRAIN;

bool Locomotive::init() {
    LOGFN();
    if (!Node::init()) return false;
    
    auto body = Sprite::create("PatternTrain/train_pattern_front_block.png");
    Size size = body->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    body->setPosition(size/2);
    
    addChild(body);
    
    return true;
}

END_NS_PATTERNTRAIN;
