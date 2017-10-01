//
//  Box.cpp
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
//

#include "Box.hpp"
#include "FreightCar.hpp"
#include "Slot.hpp"

BEGIN_NS_PATTERNTRAIN;

bool Box::init() {
    LOGFN();
    if (!Node::init()) return false;

    onTouchBegan = nullptr;
    onTouchEnded = nullptr;
    _deco = nullptr;
    _duplicated = false;
    _enableTouch = true;
    return true;
}

int Box::_index = 1;

void Box::loadOnSlot(Slot *targetSlot) {
    LOGFN();
    
    if (!targetSlot->_replaceable) return;
    
    _targetSlot = targetSlot;
    targetSlot->_boxInSlot = this;
    
    auto worldpos = this->convertToWorldSpace(this->getContentSize()/2);
    auto currentPos = targetSlot->convertToNodeSpace(worldpos);
    
    auto targetWorldPos = targetSlot->convertToWorldSpace(targetSlot->_pos);
    _loading = true;
    _originZIndex = getLocalZOrder();
    
    this->runAction(
        Sequence::create(
            MoveTo::create(0.1, targetWorldPos),
            //DelayTime::create(0.25),
            CallFunc::create([this](){
                this->retain();
                this->removeFromParent();
                this->_targetSlot->addChild(this);
                this->setPosition(this->_targetSlot->_pos);
                this->release();
            }),
            nullptr
        )
    );
  
};

void Box::unload(Node *ground) {
    LOGFN();
    if (_loading && _targetSlot) _targetSlot->_boxInSlot = nullptr;
    _loading = false;
    
    auto pos = this->convertToWorldSpace(this->getContentSize()/2);
    this->setPosition(pos);
    this->retain();
    this->removeFromParent();
    ground->addChild(this);
    this->release();
}

void Box::setGlobalZOrderManual(float order){
    _body->setGlobalZOrder(order+1);
    if (_deco != nullptr) _deco->setGlobalZOrder(order+2);
}

void Box::setType(string letter, string boxPath, PatternTrainProblemBank::shape newShape, bool setAnswer) {
    LOGFN();
    _assignedLetter = letter;
    _assignedIndex = Box::_index++;
    
    switch(newShape) {
        case PatternTrainProblemBank::shape::FIGURE :
        case PatternTrainProblemBank::shape::NUMBERALPHABET :
            _body = Sprite::create("PatternTrain/"+boxPath+".png");
            break;
        case PatternTrainProblemBank::shape::SIZE :
            if (boxPath.find("_downscale1") != string::npos) {
                TodoUtil::replaceAll(boxPath, "_downscale1", "");
                _deco = Sprite::create("PatternTrain/"+boxPath+".png");
                _deco->setScale(0.7);
            } else if (boxPath.find("_downscale2") != string::npos) {
                TodoUtil::replaceAll(boxPath, "_downscale2", "");
                _deco = Sprite::create("PatternTrain/"+boxPath+".png");
                _deco->setScale(0.4);
            } else {
                _deco = Sprite::create("PatternTrain/"+boxPath+".png");
            }
            _body = Sprite::create("PatternTrain/train_block_shape.png");
            _body->addChild(_deco);
            _deco->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _deco->setPosition(_body->getContentSize()/2);
            break;
    }
    
    if (setAnswer) {
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesBegan = [this](const vector<Touch*>& touches, Event* E) {
            if (!_enableTouch) return false;
            
            for (int i = 0; i<touches.size(); i++) {
                auto T = touches[i];
                auto P = getParent();
                auto pos = P->convertToNodeSpace(T->getLocation());
                if (this->getBoundingBox().containsPoint(pos)) {
                    //CCLOG("touchbegan: %s", _assignedLetter.c_str());
                    _touching[_assignedIndex] = T->getID()+1;
                    if (onTouchBegan) onTouchBegan();
                    return true;
                }
            }
            return false;
        };
        
        listener->onTouchesMoved = [this](const vector<Touch*>& touches, Event* E) {
            if (!_enableTouch) return;
            if (!_touching[_assignedIndex]) return;

            for (int i = 0; i<touches.size(); i++) {

                auto T = touches[i];
                if (_touching[_assignedIndex] != T->getID()+1) continue;
                //CCLOG("touchmoved: %s", _assignedLetter.c_str());

                auto P = getParent();
                auto pl = P->convertToNodeSpace(T->getPreviousLocation());
                auto cl = P->convertToNodeSpace(T->getLocation());
                auto delta = cl-pl;
                this->setPosition(this->getPosition()+delta);
                if (onTouchMoved) onTouchMoved();
            }
        };
        
        listener->onTouchesEnded = [this](const vector<Touch*>& touches, Event* E) {
            if (!_enableTouch) return;
            if (!_touching[_assignedIndex]) return;
            
            for (int i = 0; i<touches.size(); i++) {
                auto T = touches[i];
                if (_touching[_assignedIndex] != T->getID()+1) continue;

                auto P = getParent();
                auto pos = P->convertToNodeSpace(T->getLocation());
                //CCLOG("touchend#2: %s", _assignedLetter.c_str());
                if (onTouchEnded) onTouchEnded();
                _touching[_assignedIndex] = 0;
            }
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
    } else {
        setScale(0.88);
    }
    
    Size size = _body->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _body->setPosition(size/2);
    
    addChild(_body);

}

END_NS_PATTERNTRAIN;
