//
//  FreightCar.cpp
//  KitkitSchool
//
//  Created by JungJaehun on 08/09/2017.
//
//

#include "FreightCar.hpp"
#include "Box.hpp"
#include "Slot.hpp"

BEGIN_NS_PATTERNTRAIN;

const float slotHeight = 330;
const float leftMargin = 226;
const float blank = 13;

bool FreightCar::init() {
    LOGFN();
    if (!Node::init()) return false;
    _spacePointer = 0;
    return true;
}

void FreightCar::setType(int carLength) {
    
    Sprite *body;
    switch(carLength) {
        case 2:
            _carType = FOR2BOX;
            body = Sprite::create("PatternTrain/train_pattern_base_2blocks.png");
            break;
        case 3:
            _carType = FOR3BOX;
            body = Sprite::create("PatternTrain/train_pattern_base_3blocks.png");
            break;
    }
    Size size = body->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(body);
    body->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    auto connector = Sprite::create("PatternTrain/train_pattern_connector.png");
    connector->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    body->addChild(connector);
    connector->setPosition(0,60);
    
    for (int i=0; i<carLength; i++) {
        auto slot = Slot::create();
        addChild(slot);
        slot->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        
        slot->setPosition(leftMargin+(blank+slot->getContentSize().width)*i,slotHeight);
        _slotsInFreightCar.push_back(slot);
    }
}

void FreightCar::loadBox(Box *newBox, int index) {
    
    auto targetSlot = _slotsInFreightCar[index];
    
    targetSlot->_hasBox = true;
    targetSlot->_boxInSlot = newBox;
    targetSlot->addChild(newBox,-1);
    newBox->setPosition(0,0);
    //if (newBox->isBlank) targetSlot->_replaceable = true;
    /*
     auto slotPos = slot->getPosition();
     CCLOG("slot xpos: %f", slotPos.x);
     CCLOG("slot ypos: %f", slotPos.y);
     
     auto boxPos = newBox->getPosition();
     CCLOG("box xpos: %f", boxPos.x);
     CCLOG("box ypos: %f", boxPos.y);
     break;
     */
    
    
}





END_NS_PATTERNTRAIN;
