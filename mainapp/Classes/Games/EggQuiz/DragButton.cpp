//
//  DragButton.cpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#include "DragButton.hpp"

BEGIN_NS_EGGQUIZ;

DragButton* DragButton::create(string label, Size size, float fontSize, vector<Slot*> slots, function<void()> onTouchMoved, function<void()> onTouchEnded) {
    DragButton *dragButton = new (std::nothrow) DragButton();
    if (dragButton && dragButton->initWithSize(label, size, fontSize, slots, onTouchMoved, onTouchEnded))
    {
        dragButton->autorelease();
        return dragButton;
    }
    CC_SAFE_DELETE(dragButton);
    return nullptr;
}

bool DragButton::initWithSize(string label, Size size, float fontSize, vector<Slot*> slots, function<void()> onTouchMoved, function<void()> onTouchEnded) {
    _holding = false;
    _targetSlot = nullptr;
    
    setCascadeOpacityEnabled(true);
    setName("dragButton");
    
    _shadow = Scale9Sprite::create(partsPath+"pretest-answer-normal-shade.png");
    _shadow->setPreferredSize(size);
    _shadow->setPosition(_shadow->getContentSize()/2+Size(0,-20));
    addChild(_shadow);
    
    _body = Scale9Sprite::create(partsPath+"pretest-answer-normal.png");
    _body->setPreferredSize(size);
    _body->setPosition(_body->getContentSize()/2);
    addChild(_body);
    
    _active = Scale9Sprite::create(partsPath+"pretest-answer-active.png");
    _active->setPreferredSize(size);
    _active->setPosition(_active->getContentSize()/2);
    addChild(_active);
    _active->setVisible(false);
    
    _label = TodoUtil::createLabelMultilineToFitWidth(label, fontSize, size, FONT_NORMAL, FONT_COLOR, TextHAlignment::CENTER);
    _label->setPosition(_body->getContentSize()/2+Size(0,fontSize/10));
    if (_label->getName() == "multiline") _label->setPosition(_label->getPosition()+Vec2(0,fontSize*0.4));
    addChild(_label);
    
    setContentSize(_body->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this, label, size, fontSize, slots, onTouchMoved, onTouchEnded](Touch* T, Event* E) {
        if (_holding) return false;
        
        auto P = this->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos)) {
            
            if (_duplicate) {
                auto button = DragButton::create(label, size, fontSize, slots, onTouchMoved, onTouchEnded);
                button->setOriginalPos(_originPos);
                button->setLabel(_label->getString());
                P->addChild(button);
                button->fadeIn();
            }
            
            P->reorderChild(this, this->getLocalZOrder());
            _shadow->setPosition(_shadow->getContentSize()/2+Size(0,-40));
            _body->setPosition(_body->getContentSize()/2+Size(0,20));
            _label->setPosition(_label->getPosition()+Vec2(0,20));
            _holding = true;
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this, slots, onTouchMoved](Touch* T, Event* E) {
        if (!_holding) return;
        auto P = this->getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        
        if (onTouchMoved) onTouchMoved();
        
    };
    
    listener->onTouchEnded = [this, slots, onTouchEnded](Touch* T, Event* E) {
        if (!_holding) return;
        _holding = false;
        
        auto P = this->getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        
        _shadow->setPosition(_shadow->getContentSize()/2+Size(0,-20));
        _body->setPosition(_body->getContentSize()/2);
        _label->setPosition(_label->getPosition()+Vec2(0,-20));
        
        for (auto it : slots) {
            it->disableOver();
            
            if (it->getBoundingBox().containsPoint(cl)) {
                if (it->_targetButton && it->_targetButton->_label != _label) {
                    auto buttonToMove = it->_targetButton;
                    if (this->_targetSlot) {
                        buttonToMove->runAction(MoveTo::create(0.1, this->_targetSlot->getPosition()));
                        this->_targetSlot->registerButton(buttonToMove);
                        it->registerButton(this);
                    } else {
                        buttonToMove->runAction(MoveTo::create(0.1,buttonToMove->_originPos));
                        it->releaseButton();
                    }
                }
                if (this->_targetSlot) this->_targetSlot->releaseButton();
                it->registerButton(this);
                this->runAction(MoveTo::create(0.1, it->getPosition()));
                onTouchEnded();
                return;
            }
        }
        
        if (_targetSlot) _targetSlot->releaseButton();
        
        runAction(Sequence::create(
                                   CallFunc::create([this](){ _holding = true; })
                                   , MoveTo::create(0.1, _originPos)
                                   , CallFunc::create([this](){ _holding = false; })
                                   , nullptr
                                   ));
        
        onTouchEnded();
    };
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void DragButton::setLabel(string label) {
    _label->setString(label);
}

void DragButton::setOriginalPos(Vec2 pos) {
    _originPos = pos;
    this->setPosition(pos);
}

void DragButton::fadeIn() {
    setOpacity(0);
    _label->setOpacity(0);
    runAction(Sequence::create(DelayTime::create(0.3), FadeIn::create(0.5), nullptr));
    _label->runAction(Sequence::create(DelayTime::create(0.3), FadeIn::create(0.5), nullptr));
}

void DragButton::fadeOut() {
    runAction(Sequence::create(FadeOut::create(0.3), CallFunc::create([this](){this->removeFromParent();}), nullptr));
    _label->runAction(Sequence::create(FadeOut::create(0.3), nullptr));
}

void DragButton::setActive() {
    _active->setVisible(true);
    _shadow->setVisible(true);
}

void DragButton::disableDuplicate() {
    _duplicate = false;
    
}


END_NS_EGGQUIZ;
