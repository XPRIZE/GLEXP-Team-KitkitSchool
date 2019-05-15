//
//  FreightCar.cpp
//  KitkitSchool
//
//  Created by JungJaehun on 08/09/2017.
//
//

#include "Slot.hpp"
#include "PatternTrainBox.hpp"

BEGIN_NS_PATTERNTRAIN;

bool Slot::init() {
    LOGFN();
    if (!Node::init()) return false;
    
    setContentSize(Size(234,238));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    /*
    auto testSlot = Sprite::create("PatternTrain/train_pattern_empty_spot.png");
    testSlot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(testSlot);
     */
    auto pos = Vec2(0,0);
    setPosition(pos);

    _hasBox = false;
    _pos = pos;
    _boxInSlot = nullptr;
    _replaceable = false;
    return true;
}

void Slot::setBlank(string letter) {
    _replaceable = true;
    _correctAnswer = letter;
    auto blankBackground = Sprite::create("PatternTrain/train_pattern_empty_spot.png");
    addChild(blankBackground);
    auto size = blankBackground->getContentSize();
    CCLOG("blank width %f", size.width);
    CCLOG("blank height %f", size.height);
}

END_NS_PATTERNTRAIN;
