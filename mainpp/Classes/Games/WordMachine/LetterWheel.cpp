//
//  LetterWheel.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 6/28/16.
//
//

#include "LetterWheel.hpp"
#include "Managers/GameSoundManager.h"

const bool __debugLetterWheel = false;


LetterWheel::~LetterWheel()
{
    CC_SAFE_RELEASE(_stopCallback);
    
}

bool LetterWheel::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _toStop = false;
    _stopCallback = nullptr;
    _effectID = -1;

    
    GameSoundManager::getInstance()->preloadEffect("WordMachine/c1.m4a");
    GameSoundManager::getInstance()->preloadEffect("WordMachine/c3.m4a");


    return true;
}

void LetterWheel::update(float delta)
{
    Node::update(delta);
    
    _velocity += _accel*delta;
    if (_toStop && !_stopping) {
        if (_slowdownTimer>0) {
            _slowdownTimer-=delta;
        } else {
            _accel = _initVelocity*-0.2;
            if (_velocity<_initVelocity/2) {
                _velocity = _initVelocity/2;
                _accel = 0;
            }
        }
        
    }
    
    float frameOffset = delta*_velocity;

    if (_stopping && _wheelOffset>0) {
        frameOffset = _wheelOffset;
        unscheduleUpdate();
        if (_effectID>=0) GameSoundManager::getInstance()->stopEffectSound(_effectID);
        GameSoundManager::getInstance()->playEffectSound("WordMachine/c1.m4a");
        if (_stopCallback) {
            _stopCallback->execute();
        }
    }
    
    _wheelOffset-=frameOffset;

    if (_toStop && _currentLetterIndex==_stopLetterIndex && _slowdownTimer<=0) {
        if (_wheelOffset<0 || _stopping) {
            if (!_stopping) {
                _stopping = true;
                _accel = -_velocity*5.0;
            }
        }
    }
    else {
        while (_wheelOffset < -_letterHeight/2) {
            if (_toStop && _currentLetterIndex==_stopLetterIndex && _slowdownTimer<=0) {
                // NB(xenosoz, 2016): Prevent the spinner to skip the desired letter
                //   even if the velocity is too high.
                while (_wheelOffset < -_letterHeight/2) {
                    // NB(xenosoz, 2016): Recover the frame offset.
                    frameOffset -= _letterHeight;
                    _wheelOffset += _letterHeight;
                }
                break;
            }

            _wheelOffset+=_letterHeight;
            _currentLetterIndex = (_currentLetterIndex+1)%_wheelLength;

            if (_playTick) {
                if (_effectID>=0) GameSoundManager::getInstance()->stopEffectSound(_effectID);
                _effectID = GameSoundManager::getInstance()->playEffectSound("WordMachine/c3.m4a");
            }
            if (__debugLetterWheel) {
                _currentLabel->setString(_wheelString.substr(_currentLetterIndex, 1));
            }
        }
    }
    
    for (auto l : _lables) {
        auto p = l->getPosition();
        auto p2 = p+Vec2(0, -frameOffset);

        if (p2.y < -_letterHeight*2) {
            p2.y+= _letterHeight*_labelNum;
            
            _headLetterIndex = (_headLetterIndex+1)%_wheelLength;
            auto removingLetterIndex = l->getTag();
            auto nextLetterIndex = (removingLetterIndex+_labelNum)%_wheelLength;
            l->setTag(nextLetterIndex);
            l->setString(_wheelString.substr(nextLetterIndex, 1));
        }
        else if (p2.y > _letterHeight*2) {
            p2.y-= _letterHeight*_labelNum;
            
            _headLetterIndex = (_headLetterIndex-1+_wheelLength)%_wheelLength;
            auto removingLetterIndex = l->getTag();
            auto nextLetterIndex = (removingLetterIndex-_labelNum+_wheelLength)%_wheelLength;
            l->setTag(nextLetterIndex);
            l->setString(_wheelString.substr(nextLetterIndex, 1));
        }
        
        l->setPosition(p2);
        
        
        // scaling to emulate wheel
        float centerDist = fabsf(p2.y-_centerY);
        float ratio = MAX(0.0, MIN(1.0, (_centerY-centerDist) / _centerY + 0.2));
        float scale = _skew + (1.0-_skew)*ratio;
        l->setScaleY(scale);
        
        
    }
    
    
}


void LetterWheel::setup(std::string wheelString, Size viewSize, float numShowingLetters, float fontSize, float skew, bool playTick)
{
    

    _playTick = playTick;
    
    _wheelString = wheelString;
    _wheelLength = wheelString.length();
    
    setContentSize(viewSize);
    _centerX = viewSize.width/2;
    _centerY = viewSize.height/2;
    _skew = skew;
    
    auto layer = LayerColor::create(Color4B::BLUE, viewSize.width, viewSize.height);

    auto clip = ClippingNode::create(layer);
    
    auto view = Node::create();
    
    if (__debugLetterWheel) {
        addChild(layer);
        addChild(view);
    } else {
        addChild(clip);
        clip->addChild(view);
    }
    
    
    _letterHeight = viewSize.height / numShowingLetters;
    
    int midIndex = (int)ceil(numShowingLetters);
    _labelNum = midIndex*2+1;

    

    _headLetterIndex = random(0, _wheelLength-1);
    _currentLetterIndex = (_headLetterIndex+midIndex)%_wheelLength;
    _wheelOffset = 0.0;
    
    _lables.clear();
    for (int i=0; i<_labelNum; i++) {
        auto l = Label::createWithTTF("", "fonts/OpenSans-Bold.ttf", fontSize);
        _lables.push_back(l);
        
        l->setTextColor(Color4B::BLACK);
        l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        l->setPosition(viewSize.width/2, (i-midIndex)*_letterHeight+viewSize.height/2);
        int letterIndex =  (_headLetterIndex+i)%_wheelLength;
        
        l->setString(_wheelString.substr(letterIndex, 1));
        l->setTag(letterIndex);
        
        
        view->addChild(l);
    }
    

    if (__debugLetterWheel) {
        _currentLabel = Label::createWithTTF("", "fonts/OpenSans-Bold.ttf", fontSize);
        _currentLabel->setString(_wheelString.substr(_currentLetterIndex, 1));
        _currentLabel->setPosition(Vec2(viewSize.width, viewSize.height/2));
        view->addChild(_currentLabel);
    }

}



void LetterWheel::start(float velocity)
{
    _initVelocity = velocity;
    _velocity = velocity;
    _accel = 0;
    
    _stopLetterIndex = -1;
    _toStop = false;
    _stopping = false;
    _slowdownTimer = 0;

    CC_SAFE_RELEASE(_stopCallback);
    scheduleUpdate();
}


void LetterWheel::stop(char targetChar, float slowdownAfter, CallFunc *callback)
{
    
    _stopLetterIndex = _wheelString.find(targetChar);
    _toStop = true;
    _stopping = false;
    _slowdownTimer = slowdownAfter;
    
    _stopCallback = callback;
    CC_SAFE_RETAIN(_stopCallback);
    
}
