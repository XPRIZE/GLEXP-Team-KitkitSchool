//
//  AnswerPiece.cpp
//  todomath
//
//  Created by Gunho Lee on 3/21/17.
//
//

#include "AnswerPiece.hpp"
#include "TrainCar.hpp"

#include "NumberTrainHeader.h"


bool AnswerPiece::init()
{
    if (!Node::init()) return false;
    
    _snapped = false;
    _enableTouch = true;
    
    onTouchBegan = nullptr;
    onTouchEnded = nullptr;
    
    _targetSlot = nullptr;
    
    
    
    _face = Sprite::create("NumberTrain/smallest_largest_1_answerblcok_surface.png");
    
    Size size = _face->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _face->setPosition(size/2);
    
    
    _body = Sprite::create("NumberTrain/smallest_largest_2_answerblcok_depth.png");
    _body->setPosition(size/2);
    
    _shadow = Sprite::create("NumberTrain/smallest_largest_3_answerblcok_shadow.png");
    _shadow->setPosition(size/2);
    
    addChild(_shadow);
    addChild(_body);
    addChild(_face);
    
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        if (!_enableTouch) return false;
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
            
            
            this->setPicked(true);
            
            if (onTouchBegan) onTouchBegan();
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        if (!_enableTouch) return;
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        if (!_enableTouch) return;
        setPicked(false);
        if (onTouchEnded) onTouchEnded();
        
        
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
    
    
}

Node* AnswerPiece::createFace(int number)
{
    const string fontName = "fonts/mukta-bold.ttf";
    
    auto faceSize = Size(222, 244)  - Size(22, 44) - Size(30, 30);
    if (number>=10000) {
        auto shape = number/10000;
        string filename;
        
        switch (shape) {
            case 1: filename = "circustrain_shape_circle_1.png"; break;
            case 2: filename = "circustrain_shape_rectangle_1.png"; break;
            case 3: filename = "circustrain_shape_square_1.png"; break;
            case 4: filename ="circustrain_shape_triangle_1.png"; break;
        }
        
        auto scalePT = number%100;
        if (scalePT==0) scalePT = 100;
        auto scale = scalePT / 100.0;
        
        auto sprite = Sprite::create("NumberTrain/"+filename);
        auto spriteSize = sprite->getContentSize();
        auto adjScale = MIN(faceSize.width/spriteSize.width, faceSize.height/spriteSize.height);
        
        sprite->setScale(scale*adjScale);
        return sprite;
        
    } else {

        auto l = TodoUtil::createLabel(TodoUtil::itos(number), 100, faceSize, fontName, Color4B(79, 67, 61, 255), TextHAlignment::CENTER);
        return l;
        
    }
}


void AnswerPiece::setNumber(int number)
{
    
    
    _number = number;
    
    auto size = getContentSize();
    
    auto faceTexture = createFace(number);
    faceTexture->setPosition(size/2+Size(0, 11));
    _face->addChild(faceTexture);
    
}

void AnswerPiece::setPicked(bool picked)
{
    Size size = getContentSize();
    
    if (picked) {
        
        _face->setPosition(size.width/2, size.height/2+5);
        _body->setPosition(size.width/2, size.height/2+5);
        _shadow->setPosition(size.width/2+5, size.height/2-5);
        _shadow->setVisible(true);
        _body->setVisible(true);
        
    } else {
        _face->setPosition(size/2);
        _body->setPosition(size/2);
        _shadow->setPosition(size/2);
        
    }
}

void AnswerPiece::snapTarget(TrainCar *target)
{
    _targetSlot = target;
    
    target->_cardInSlot = this;
    
    
    auto worldpos = this->convertToWorldSpace(this->getContentSize()/2);
    auto currentPos = target->convertToNodeSpace(worldpos);
    
    auto targetWorldPos = target->convertToWorldSpace(target->_slotPos);
    
    setPicked(false);
    _shadow->setVisible(false);
    _body->setVisible(false);
    _snapped = true;
    
    
    this->runAction(Sequence::create(MoveTo::create(0.1, targetWorldPos), CallFunc::create([this](){
        
        //this->getParent()->reorderChild(this, this->getLocalZOrder());
        
        this->retain();
        this->removeFromParent();
        this->setPosition(this->_targetSlot->_slotPos);
        this->_targetSlot->addChild(this);
        this->release();
        
    }), nullptr));
    
    
    
    
}

void AnswerPiece::unSnap(cocos2d::Node *root)
{
    if (_snapped && _targetSlot) _targetSlot->_cardInSlot = nullptr;
    _snapped = false;
    
    _shadow->setVisible(true);
    _body->setVisible(true);
    
    auto pos = this->convertToWorldSpace(this->getContentSize()/2);
    this->setPosition(pos);
    
    //this->getParent()->reorderChild(this, this->getLocalZOrder());
    this->retain();
    this->removeFromParent();
    root->addChild(this);
    this->release();
}
