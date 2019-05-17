//
//  LRGuideView.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 08/08/2018.
//

#include "LRGuideView.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"

namespace
{
    const string kPrefixPath = "lrcomprehension/common/";
    const float kAdjustValueX = 12.f;
}

LRGuideView::LRGuideView():
_bSoundType(false),
_bActive(false)
{
    
}

LRGuideView* LRGuideView::createWithSound(string soundPath)
{
    LRGuideView *pRet = new(std::nothrow) LRGuideView();
    if (pRet && pRet->init(true, soundPath, ""))
    {
        pRet->setSoundPath(soundPath);
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

LRGuideView* LRGuideView::createWithText(string textContent)
{
    LRGuideView *pRet = new(std::nothrow) LRGuideView();
    if (pRet && pRet->init(false, "", textContent))
    {
        pRet->setSoundType(false);
        pRet->setTextContent(textContent);
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool LRGuideView::init(bool isSoundType, string soundPath, string textContent)
{
    if (!Node::init()) return false;
    
    _bSoundType = isSoundType;
    _soundPath = soundPath;
    _textContent = textContent;
    
    auto winSize = Director::getInstance()->getWinSize();
    
    auto background = Button::create(kPrefixPath + "comprehension-textbody-bg.png", kPrefixPath + "comprehension-textbody-bg.png");
    this->setContentSize(background->getContentSize());
    background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    background->setPosition(this->getContentSize()/2);
    background->addClickEventListener([this](Ref*) {
        
    });
    this->addChild(background);
    
    if (_bSoundType)
    {
        _speakerButton = LRSoundButton::createLarge(_soundPath);
        _speakerButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _speakerButton->setPosition(background->getContentSize()/2);
        background->addChild(_speakerButton);
    }
    else
    {
        _textLabel = TodoUtil::createLabelMultilineToFit(_textContent, 72.f, Size(1930, 844), FONT_MUKTA_BOLD, Color4B(77, 77, 77, 255));
        _textLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _textLabel->setPosition(background->getContentSize()/2);
        this->addChild(_textLabel);
    }
 
    //auto leftButton = Button::create(kPrefixPath + "comprehension-textbody-prevbutton.png", kPrefixPath + "comprehension-textbody-prevbutton.png");
    leftButton = Button::create(kPrefixPath + "comprehension-textbody-prevbutton.png");
    leftButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    leftButton->setPosition(Vec2(0.f + kAdjustValueX, this->getContentSize().height/2));
    leftButton->addClickEventListener([this](Ref*) {
        showPanel(true);
        
        if (onClickLeftButton != nullptr)
            onClickLeftButton();
    });
    this->addChild(leftButton);

    //auto rightButton = Button::create(kPrefixPath + "comprehension-textbody-nextbutton.png", kPrefixPath + "comprehension-textbody-nextbutton.png");
    rightButton = Button::create(kPrefixPath + "comprehension-textbody-nextbutton.png");
    rightButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    rightButton->setPosition(Vec2(this->getContentSize().width - kAdjustValueX, this->getContentSize().height/2));
    rightButton->addClickEventListener([this](Ref*) {
        showPanel(false);
        
        if (onClickRightButton != nullptr)
            onClickRightButton();
        
        GameSoundManager::getInstance()->stopBGM();
        GameSoundManager::getInstance()->stopAllEffects();
        for (const auto b : LRSoundButton::soundButtons)
        {
            b->setPressed(false);
        }
    });
    this->addChild(rightButton);
    
    showPanel(true);
    
    return true;
}

void LRGuideView::showPanel(bool isShow)
{
    leftButton->setVisible(!isShow);
    rightButton->setVisible(isShow);
}

