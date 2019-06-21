//
//  LRSoundButton.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 21/09/2018.
//

#include <Managers/VoiceMoldManager.h>
#include "LRSoundButton.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"

namespace
{
    string kBasePath = "lrcomprehension/sounds/";
    string kDurationDataPath = kBasePath + "durations.tsv";
}

vector<LRSoundButton*> LRSoundButton::soundButtons = vector<LRSoundButton*>();

bool LRSoundButton::init(bool isLarge, string soundName)
{
    if (Node::init() == false)
        return false;
    
    this->_soundName = soundName;
    
    initData();
    
    if (isLarge)
        _defaultImage = Sprite::create("lrcomprehension/common/comprehension-speaker-b-normal.png");
    else
        _defaultImage = Sprite::create("lrcomprehension/common/comprehension-speaker-normal.png");
    
    this->setContentSize(_defaultImage->getContentSize());
    _defaultImage->setPosition(this->getContentSize()/2);
    this->addChild(_defaultImage);
    
    if (isLarge)
        _playingImage = Sprite::create("lrcomprehension/common/comprehension-speaker-b-active.png");
    else
        _playingImage = Sprite::create("lrcomprehension/common/comprehension-speaker-active.png");
    
    _playingImage->setPosition(this->getContentSize()/2);
    _playingImage->setVisible(false);
    this->addChild(_playingImage);
    
    return true;
}

void LRSoundButton::initData()
{
    std::string rawString = FileUtils::getInstance()->getStringFromFile(kDurationDataPath);
    auto data = TodoUtil::readTSV(rawString);
    
    for (auto row : data) {
        if (TodoUtil::trim(row[0]).size() <= 0) continue;
        if (row.size()==1) continue;
        auto row1Vec = TodoUtil::split(row[1], ':');
        TodoUtil::replaceAll(row1Vec[2], ".", "");
        auto rowDuration = (float)TodoUtil::stoi(row1Vec[2])/100;
        _durationMap[TodoUtil::trim(row[0])] = rowDuration;
    }
}

LRSoundButton* LRSoundButton::createLarge(string soundName)
{
    auto button = new LRSoundButton();
    if (button && button->init(true, soundName))
    {
        soundButtons.push_back(button);
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}

LRSoundButton* LRSoundButton::createSmall(string soundName)
{
    auto button = new LRSoundButton();
    if (button && button->init(false, soundName))
    {
        soundButtons.push_back(button);
        button->autorelease();
        return button;
    }
    CC_SAFE_DELETE(button);
    return nullptr;
}


void LRSoundButton::onEnter()
{
    Node::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        /*
        for (const auto b : soundButtons)
        {
            if (b->isPlaying())
                return false;
        }
        
        if (_isPlaying)
            return false;
        Vec2 touchPosition = this->getParent()->convertToNodeSpace(touch->getLocation());
        if (this->getBoundingBox().containsPoint(touchPosition))
        {
            playSound();
            return true;
        }
        return false;
         */
        
        Vec2 touchPosition = this->getParent()->convertToNodeSpace(touch->getLocation());
        if (this->getBoundingBox().containsPoint(touchPosition))
        {
            GameSoundManager::getInstance()->stopBGM();
            GameSoundManager::getInstance()->stopAllEffects();
            for (const auto b : soundButtons)
            {
                b->setPressed(false);
            }
            playSound();
        }

        return false;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
 // Implementation of tts for this game module
void LRSoundButton::playSound()
{
    _isPlaying = true;
    _defaultImage->setVisible(false);
    _playingImage->setVisible(true);
   VoiceMoldManager::shared()->speak(_soundName);
    DELAYED_CALLFUNC(_durationMap[_soundName], {
        _isPlaying = false;
        _defaultImage->setVisible(true);
        _playingImage->setVisible(false);
    });
}

bool LRSoundButton::isPlaying()
{
    return _isPlaying;
}

void LRSoundButton::setPressed(bool isPressed)
{
    _isPlaying = isPressed;
    _defaultImage->setVisible(!isPressed);
    _playingImage->setVisible(isPressed);
}
