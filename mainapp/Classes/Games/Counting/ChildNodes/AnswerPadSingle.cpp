//
//  AnswerPadSingle.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 6/26/16.
//
//

#include "AnswerPadSingle.hpp"

#include "Utils/TodoUtil.h"

#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"

namespace AnswerPadSingleSpace {
    const char* touchEffect = "Counting/paneltouch.m4a";
    const char* formalFont = "fonts/Aileron-Regular.otf";
}

using namespace AnswerPadSingleSpace;



bool AnswerPadSingle::init()
{
    if (!AnswerPadBase::init())
    {
        return false;
    }

    GameSoundManager::getInstance()->preloadEffect(touchEffect);
    
    
    
    auto answerPadBg = Sprite::create("Counting/counting_image_numberpad_bg.png");
    auto answerPadSize = answerPadBg->getContentSize();
    answerPadBg->setPosition(answerPadSize/2);
    
    setContentSize(answerPadSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    addChild(answerPadBg);
    
    
    _questionLabel = Label::createWithSystemFont(LanguageManager::getInstance()->isEnglish() ?  "How many?" : "Ngapi?", formalFont, 100);
    _questionLabel->setTextColor(Color4B(242, 245, 240, 255));
    _questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _questionLabel->setPosition(Vec2(150, answerPadSize.height-129));
    addChild(_questionLabel);
    
    
    _answerLabel = Label::createWithSystemFont("2", formalFont, 500);
    _answerLabel->setTextColor(Color4B(242, 245, 240, 255));
    _answerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _answerLabel->setPosition(answerPadSize.width/2, 950);
    _answerLabel->setVisible(false);
    addChild(_answerLabel);
    

    
    
    _panelNode = Node::create();
    auto panelSize = Size(184, 184);
    _panelNode->setContentSize(Size(panelSize.width*5+10*4, panelSize.height*2+10));
    _panelNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _panelNode->setPosition(Vec2(answerPadSize.width/2, 129));
    addChild(_panelNode);
    
    const auto normalColor = Color3B(59, 140, 56);
    const auto highlightColor = Color3B(242, 245, 240);
    
    for (int i=1; i<=10; i++) {
        float x = ((i-1)%5)*(panelSize.width+10)+panelSize.width/2;
        float y = 10+panelSize.height*2 - ( ((i-1)/5)*(panelSize.height+10)+panelSize.height/2   );
        
        auto btn = ui::Button::create();
        btn->loadTextures("Counting/counting_button_numberpad_normal.png", "Counting/counting_button_numberpad_touch.png");
        btn->setTitleFontSize(160);
        btn->setTitleColor(normalColor);
        btn->setTitleText(TodoUtil::itos(i));
        
        btn->setPosition(Vec2(x, y));
        btn->setTag(i);
        btn->addTouchEventListener([btn, normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {

            if (!_touchEnabled) return;
            
            switch (event) {
                case Widget::TouchEventType::BEGAN:
                    btn->setTitleColor(highlightColor);
                    break;
                case Widget::TouchEventType::CANCELED:
                case Widget::TouchEventType::ENDED:
                    btn->setTitleColor(normalColor);
                    break;
                case Widget::TouchEventType::MOVED:
                    
                    btn->setTitleColor(btn->isHighlighted() ? highlightColor : normalColor);
                    break;
                default:
                    break;
            }
        });
        
        btn->addClickEventListener([btn, this](Ref* sender) {
            if (!_touchEnabled) return;
            
            GameSoundManager::getInstance()->playEffectSound(touchEffect);
            auto ans = btn->getTag();
            _answerLabel->setString(TodoUtil::itos(ans));
            _answerLabel->setVisible(true);
            
            _panelNode->setVisible(false);
            _enterButton->setVisible(true);
            
        });
        
        _panelNode->addChild(btn);
    }

    
    _enterButton = Button::create();
    _enterButton->setAnchorPoint(_panelNode->getAnchorPoint());
    _enterButton->loadTextures("Counting/counting_button_confirm-large_normal.png", "Counting/counting_button_confirm-large_touch.png");
    _enterButton->setPosition(_panelNode->getPosition());
    _enterButton->setVisible(false);
    _enterButton->addClickEventListener([this](Ref*) {
        if (!_touchEnabled) return;
        _answerCallback(TodoUtil::stoi(_answerLabel->getString()));
    });
    
    addChild(_enterButton);
    
    

    return true;
}


void AnswerPadSingle::clearAnswer()
{
    _answerLabel->setString("");
    _answerLabel->setVisible(false);
    _enterButton->setVisible(false);
    _panelNode->setVisible(true);
    
}
