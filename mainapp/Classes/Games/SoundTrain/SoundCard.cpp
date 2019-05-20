//
//  SoundCard.cpp
//  KitkitSchool
//
//  Created by JungJaehun on 05/09/2017.
//
//

#include "SoundCard.hpp"
#include "SoundTrain.hpp"

BEGIN_NS_SOUNDTRAIN;

const string fontName = "fonts/mukta-bold.ttf";

bool SoundCard::init() {
    if (!Node::init()) return false;
    
    _targetTrain = nullptr;
    onTouchBegan = nullptr;
    onTouchEnded = nullptr;
    _enableTouch = true;
    
    return true;
    
}

int SoundCard::_index = 1;

void SoundCard::loadOnTrain(SoundTrain *train) {
    LOGFN();
    _targetTrain = train;
    train->_cardInTrain = this;
    
    auto worldPos = this->convertToWorldSpace(this->getContentSize()/2);
    auto currentPos = train->convertToNodeSpace(worldPos);
    auto trainWorldPos = train->convertToWorldSpace(train->_slotPos);
    
    changePickUpState(false);
    _shadow->setVisible(false);
    _body->setVisible(false);
    _loading = true;
    
    this->runAction(Sequence::create(MoveTo::create(0.1, trainWorldPos), CallFunc::create([this](){
        //this->getParent()->reorderChild(this, this->getLocalZOrder());
        
        this->retain();
        this->removeFromParent();
        this->setPosition(this->_targetTrain->_slotPos);
        this->_targetTrain->addChild(this);
        this->release();
    }), nullptr));
    
}

void SoundCard::unload(Node *ground) {
    if (_loading && _targetTrain) _targetTrain->_cardInTrain = nullptr;
    _loading = false;
    
    _shadow->setVisible(true);
    _body->setVisible(true);
    
    auto pos = this->convertToWorldSpace(this->getContentSize()/2);
    this->setPosition(pos);
    //this->getParent()->reorderChild(this, this->getLocalZOrder());
    this->retain();
    this->removeFromParent();
    ground->addChild(this);
    this->release();
    
}

void SoundCard::changePickUpState(bool picked) {
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

void SoundCard::setType(string letter){
    _letter = letter;
    
    string facePath = "SoundTrain/train_card_surface.png";
    string bodyPath = "SoundTrain/train_card_surface.png";
    string shadowPath = "SoundTrain/train_card_surface.png";
    
    if (letter.size() > 1) {
        TodoUtil::replaceAll(facePath, "_card", "_card_long");
        TodoUtil::replaceAll(bodyPath, "_card", "_card_long");
        TodoUtil::replaceAll(shadowPath, "_card", "_card_long");
    }
    
    _face = Sprite::create(facePath);
    Size size = _face->getContentSize();
    setContentSize(size);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _face->setPosition(size/2);
    
    _body = Sprite::create(bodyPath);
    _body->setPosition(size/2);
    
    _shadow = Sprite::create(shadowPath);
    _shadow->setPosition(size/2);
    
    addChild(_shadow);
    addChild(_body);
    addChild(_face);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    // listener->setSwallowTouches(true);
    listener->onTouchesBegan = [this](const vector<Touch*>& touches, Event* E) {
        
        for (int i = 0; i<touches.size(); i++) {
            auto T = touches[i];
            auto P = getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (this->getBoundingBox().containsPoint(pos)) {
                if (!_enableTouch) return false;
                _enableTouch = false;

                CCLOG("tx:%f, ty:%f", T->getLocation().x, T->getLocation().y);
                CCLOG("ontouchbegan");
                //CCLOG("touchbegan: %s", _assignedLetter.c_str());
                _touching[_assignedIndex] = T->getID()+1;
                if (onTouchBegan) onTouchBegan();
                return true;
            }
        }
        return false;
    };
    
    listener->onTouchesMoved = [this](const vector<Touch*>& touches, Event* E) {
        if (!_touching[_assignedIndex]) return;

        for (int i = 0; i<touches.size(); i++) {
            
            auto T = touches[i];
            if (_touching[_assignedIndex] != T->getID()+1) continue;
            //CCLOG("touchmoved: %s", _assignedLetter.c_str());
            
            auto P = getParent();
            auto pl = P->convertToNodeSpace(T->getPreviousLocation());
            auto cl = P->convertToNodeSpace(T->getLocation());
            auto delta = cl-pl;
            // this->setPosition(this->getPosition()+delta);
            this->setPosition(cl);
        }
    };
    
    listener->onTouchesEnded = [this](const vector<Touch*>& touches, Event* E) {
        if (!_touching[_assignedIndex]) return;
        
        for (int i = 0; i<touches.size(); i++) {
            auto T = touches[i];
            if (_touching[_assignedIndex] != T->getID()+1) continue;
            _enableTouch = true;

            auto P = getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            //CCLOG("touchend#2: %s", _assignedLetter.c_str());
            if (onTouchEnded) onTouchEnded();
            _touching[_assignedIndex] = 0;
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto cardLetter = TodoUtil::createLabel(letter, 140, Size::ZERO, fontName, Color4B(54, 54, 54,255), TextHAlignment::CENTER);
    cardLetter->setPosition(_face->getPositionX(), _face->getPositionY()+10);
    cardLetter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(cardLetter);

}

END_NS_SOUNDTRAIN;
