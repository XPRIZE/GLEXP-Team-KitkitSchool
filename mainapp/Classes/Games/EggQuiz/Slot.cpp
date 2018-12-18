//
//  Slot.cpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#include "Slot.hpp"

BEGIN_NS_EGGQUIZ;

Slot* Slot::create(SlotSize slotSize) {
    Slot *slot = new (std::nothrow) Slot();
    if (slot && slot->initWithSize(slotSize))
    {
        slot->autorelease();
        return slot;
    }
    CC_SAFE_DELETE(slot);
    return nullptr;
}

bool Slot::initWithSize(SlotSize slotSize) {
    
    _targetButton = nullptr;
    
    string bodyPath = "";
    string overPath = "";
    
    switch(slotSize) {
        case SlotSize::Small:
            bodyPath = partsPath+"pretest-math-emptyslot-s.png";
            overPath = partsPath+"pretest-math-emptyslot-active-s.png";
            break;
        case SlotSize::Big:
            bodyPath = partsPath+"pretest-math-emptyslot.png";
            overPath = partsPath+"pretest-math-emptyslot-active.png";
            break;
        case SlotSize::Bar:
            bodyPath = partsPath+"pretest-literacy-emptyslot.png";
            overPath = partsPath+"pretest-literacy-emptyslot-active.png";
            break;
    }
    
    _body = Scale9Sprite::create(bodyPath);
    _body->setPosition(_body->getContentSize()/2);
    addChild(_body);
    
    _over = Scale9Sprite::create(overPath);
    _over->setPosition(_body->getContentSize()/2);
    addChild(_over);
    
    _over->setVisible(false);
    setName("Slot");
    setContentSize(_body->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}

void Slot::setAnswer(std::string answer) {
    _answer = answer;
}

string Slot::getAnswer() {
    return _answer;
}

void Slot::enableOver() {
    _over->setVisible(true);
}

void Slot::disableOver() {
    _over->setVisible(false);
}

void Slot::registerButton(DragButton* button) {
    _targetButton = button;
    button->_targetSlot = this;
}

void Slot::releaseButton() {
    if (_targetButton) _targetButton->_targetSlot = nullptr;
    _targetButton = nullptr;
}

END_NS_EGGQUIZ;
