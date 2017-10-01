//
//  SoundTrain.cpp
//  enumaXprize
//
//  Created by JungJaehun on 04/09/2017.
//
//

#include "SoundTrain.hpp"
#include "SoundCard.hpp"

BEGIN_NS_SOUNDTRAIN;

bool SoundTrain::init() {
    if (!Node::init()) return false;
    _cardInTrain = nullptr;
    _connectorStatus = true;
    
    return true;
}

void SoundTrain::setType(string letter) {
    _correctAnswer = letter;
    
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto trainBody = Sprite::create(letter.size() == 1 ? "SoundTrain/train_compartment_normal.png" : "SoundTrain/train_compartment_long.png");
    trainBody->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _trainBody = trainBody;
    
    _slotPos = trainBody->getContentSize()/2;
    _slotPos.y += 26;
    
    addChild(_trainBody,0);
    
    _connector = Sprite::create("SoundTrain/train_connector.png");
    _connector->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _connector->setPosition(-76,65);
    _trainBody->addChild(_connector,-1);
    
    setContentSize(_trainBody->getContentSize());
    
}

void SoundTrain::toggleConnector() {
    return;
    float delay = 1.0;
    _connectorStatus = !_connectorStatus;
    
    runAction(Sequence::create(
        CallFunc::create([this, delay](){
        _connector->runAction(EaseOut::create(MoveBy::create(delay, Vec2(_connectorStatus?-80:80,0)), 2));
        }),
        nullptr
    ));
}

END_NS_SOUNDTRAIN;
