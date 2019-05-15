//
//  PushButton.cpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#include "PushButton.hpp"

BEGIN_NS_EGGQUIZ;

PushButton* PushButton::create(Size buttonSize, float fontSize, string label, function<void(string answer)> onTouchBegan, string fontPath) {
    PushButton *pushButton = new (std::nothrow) PushButton();
    if (pushButton && pushButton->initWithSize(buttonSize, fontSize, label, onTouchBegan, fontPath))
    {
        pushButton->autorelease();
        return pushButton;
    }
    CC_SAFE_DELETE(pushButton);
    return nullptr;
}

bool PushButton::initWithSize(Size buttonSize, float fontSize, string label, function<void(string answer)> onTouchBegan, string fontPath) {
    
    _body = Scale9Sprite::create(partsPath+"pretest-answer-normal.png");
    _body->setPreferredSize(buttonSize);
    _body->setPosition(_body->getContentSize()/2);
    addChild(_body);
    
    _active = Scale9Sprite::create(partsPath+"pretest-answer-active.png");
    _active->setPreferredSize(buttonSize);
    _active->setPosition(_active->getContentSize()/2);
    addChild(_active);
    _active->setVisible(false);
    
    _label = TodoUtil::createLabelMultilineToFitWidth(label, fontSize, buttonSize, fontPath.empty() ? FONT_NORMAL : fontPath, FONT_COLOR, TextHAlignment::CENTER);
    _label->setPosition(_body->getContentSize()/2+Size(0,10));
    if (_label->getName() == "multiline") _label->setPosition(_label->getPosition()+Vec2(0,fontSize*0.4));
    addChild(_label);
    
    setContentSize(_body->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this, onTouchBegan](Touch* T, Event* E) {
        
        auto P = this->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos)) {
            this->setActive();
            onTouchBegan(_label->getString());
            return true;
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void PushButton::setImage() {
    auto fileName = imagesPath+_label->getString();
    if (fileName.find(".png")==string::npos) fileName += ".png";
    
    auto image = Sprite::create(fileName);
    image->setScale(MIN(this->getContentSize().width/image->getContentSize().width, this->getContentSize().height/image->getContentSize().height)*0.88);
    image->setPosition(_body->getContentSize()/2 + Size(0,10));
    addChild(image);
    _label->setVisible(false);
}

void PushButton::setLabel(string label) {
    _label->setString(label);
}

void PushButton::setActive() {
    _active->setVisible(true);
}

void PushButton::setDeactive() {
    _active->setVisible(false);
}

END_NS_EGGQUIZ;
