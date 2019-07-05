//
//  Speaker.cpp
//  KitkitTest
//
//  Created by JungJaehun on 18/07/2018.
//

#include "Speaker.hpp"
#include "Managers/GameSoundManager.h"
#include <Managers/VoiceMoldManager.h>

BEGIN_NS_EGGQUIZ;

const int pressButtonTag = 1;
const int pressTriggerTag = 2;

Speaker* Speaker::create(SpeakerSize speakerSize) {
    Speaker *speaker = new (std::nothrow) Speaker();
    if (speaker && speaker->initWithSize(speakerSize))
    {
        speaker->autorelease();
        return speaker;
    }
    CC_SAFE_DELETE(speaker);
    return nullptr;
}

bool Speaker::initWithSize(SpeakerSize speakerSize) {
    
    _onCompleteCallback = nullptr;
    
    string bodyPath = "";
    string overPath = "";
    
    switch(speakerSize) {
        case SpeakerSize::Small:
            bodyPath = partsPath+"pretest-speaker-normal-s.png";
            overPath = partsPath+"pretest-speaker-active-s.png";
            break;
        case SpeakerSize::Big:
            bodyPath = partsPath+"pretest-speaker-normal.png";
            overPath = partsPath+"pretest-speaker-active.png";
            break;
    }
    
    _body = Scale9Sprite::create(bodyPath);
    _body->setPosition(_body->getContentSize()/2);
    addChild(_body);
    
    _over = Scale9Sprite::create(overPath);
    _over->setPosition(_over->getContentSize()/2);
    addChild(_over);
    
    _over->setVisible(false);
    
    setContentSize(_body->getContentSize());
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setName("speaker");
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        auto P = this->getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        
        if (this->getBoundingBox().containsPoint(pos)) {
            setPressed();
            return true;
        }
        return false;
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void Speaker::setFileName(string fileName) {
    _fileName = fileName;
}

void Speaker::setPressed(float delay) {
    if (_pressDisabled) return;
    
    auto files = TodoUtil::split(_fileName, ',');
    float duration = delay;
    for (int i=0; i<files.size(); i++) {
        auto it = files.at(i);
        duration += ProblemBank::getInstance()->getDuration(it);
        if (i) duration += 1.0;
    }
    
    auto sequence = Sequence::create(
        DelayTime::create(delay),
        CallFunc::create([this](){
            _over->setVisible(true);
            _body->setVisible(false);
        }),
        DelayTime::create(duration-delay),
        CallFunc::create([this](){
            _over->setVisible(false);
            _body->setVisible(true);
        if (_onCompleteCallback) _onCompleteCallback();
        }),
        nullptr
    );
    sequence->setTag(pressTriggerTag);

    Vector<FiniteTimeAction*> actions;
    
    for (auto it : files) {
        actions.pushBack(DelayTime::create(delay));
        actions.pushBack(CallFunc::create([this, it](){
           VoiceMoldManager::shared()->speak(it);        //Implementation of TTS for speaker button
        }));
        delay += ProblemBank::getInstance()->getDuration(it)+1;
    }
    
    auto sequence2 = Sequence::create(actions);
    sequence2->setTag(pressButtonTag);
    
    this->stopActionByTag(pressButtonTag);
    this->stopActionByTag(pressTriggerTag);
    this->runAction(sequence);
    this->runAction(sequence2);
    
}

void Speaker::setPressDisabled() {
    _pressDisabled = true;
}

END_NS_EGGQUIZ;
