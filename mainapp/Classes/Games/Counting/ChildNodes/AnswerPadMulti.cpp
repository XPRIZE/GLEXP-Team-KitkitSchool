//
//  AnswerPadMulti.cpp on Jul 29, 2016
//  KitkitSchool
//
//  Extended by Taihyun Hwang 7/29/16 base on the work of
//  Created by Gunho Lee on 6/26/16.
//


#include "AnswerPadMulti.hpp"

#include "Utils/TodoUtil.h"

#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"


namespace AnswerPadMultiSpace {
    const char* touchEffect = "Counting/paneltouch.m4a";
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";
    const char* formalFont = "fonts/Aileron-Regular.otf";
}

using namespace AnswerPadMultiSpace;
using namespace std;



bool AnswerPadMulti::init()
{
    if (!AnswerPadBase::init())
    {
        return false;
    }
    
    GameSoundManager::getInstance()->preloadEffect(touchEffect);
    
    _answerDigitCount = 0;
    _answerString.clear();


    _answerPadBg = Sprite::create("Counting/counting_image_numberpad_bg.png");
    auto answerPadSize = _answerPadBg->getContentSize();
    _answerPadBg->setPosition(answerPadSize/2);
    
    setContentSize(answerPadSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    addChild(_answerPadBg);
    
    
    _questionLabel = Label::createWithSystemFont(LanguageManager::getInstance()->isEnglish() ?  "How many?" : "Ngapi?", formalFont, 100);
    _questionLabel->setTextColor(Color4B(242, 245, 240, 255));
    _questionLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _questionLabel->setPosition(Vec2(150, answerPadSize.height-65));
    addChild(_questionLabel);
    
    
    _panelNode = Node::create();

    auto panelSize = Size(306, 184);
    const int rowCount = 4;
    const int colCount = 3;
    _panelNode->setContentSize(Size(panelSize.width * colCount + 10.f * (colCount-1), panelSize.height * rowCount + 10.f));
    _panelNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _panelNode->setPosition(Vec2(answerPadSize.width/2, 129.f));
    addChild(_panelNode);
    
    const auto normalColor = Color3B(59, 140, 56);
    const auto highlightColor = Color3B(242, 245, 240);
    
    auto panelPos = [panelSize, rowCount, colCount](int row, int col)
    {
        float x = col * (panelSize.width + 10.f) + panelSize.width/2;
        float y = 10.f + panelSize.height * rowCount - (row * (panelSize.height + 10.f) + panelSize.height/2);

        return Point(x, y);
    };
    
    for (int i = 0; i <= 9; i++) {
        Point btnPos = panelPos((9-i)/colCount, colCount - (9-i)%colCount - 1);
        string normalTexture = "Counting/counting_button_numberpad_wide_1x1_normal.png";
        string touchTexture = "Counting/counting_button_numberpad_wide_1x1_touch.png";
        if (i == 0) {
            // NB(xenosoz, 2016): Zero is 2x wider than others.
            btnPos = (panelPos(rowCount - 1, 0) + panelPos(rowCount - 1, 1)) / 2.f;
            normalTexture = "Counting/counting_button_numberpad_wide_1x2_normal.png";
            touchTexture = "Counting/counting_button_numberpad_wide_1x2_touch.png";
        }

        auto btn = ui::Button::create();
        btn->loadTextures(normalTexture, touchTexture);
        btn->setTitleFontSize(160);
        btn->setTitleColor(normalColor);
        btn->setTitleText(TodoUtil::itos(i));
        
        btn->setPosition(btnPos);
        btn->setTag(i);
        btn->addTouchEventListener([btn, normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {
            
            if (!_touchEnabled)
                event = Widget::TouchEventType::CANCELED;
            
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
            if (!_touchEnabled) { return; }
            
            GameSoundManager::getInstance()->playEffectSound(touchEffect);
            auto ans = btn->getTag();
            
            if (_answerString.size() < _answerDigitCount) {
                _answerString += TodoUtil::itos(ans);
                handleAnswerStringChanged();
            }
        });
        
        if (i == 0) { _zeroButton = btn; }
        _panelNode->addChild(btn);
    }
    
    
    
    _deleteButton = ui::Button::create();
    _deleteButton->loadTextures("Counting/counting_button_numberpad_wide_1x1_normal.png",
                      "Counting/counting_button_numberpad_wide_1x1_touch.png");
    _deleteButton->setTitleFontSize(160);
    _deleteButton->setTitleColor(normalColor);
    _deleteButton->setTitleText("←");  // NB(xenosoz, 2016): "\u2190" LEFTWARDS ARROW
    
    _deleteButton->setPosition(panelPos(rowCount - 1, colCount - 1));
    _deleteButton->addTouchEventListener([normalColor, highlightColor, this](Ref* sender,Widget::TouchEventType event) {
        
        if (!_touchEnabled) return;
        
        switch (event) {
            case Widget::TouchEventType::BEGAN:
                _deleteButton->setTitleColor(highlightColor);
                break;
            case Widget::TouchEventType::CANCELED:
            case Widget::TouchEventType::ENDED:
                _deleteButton->setTitleColor(normalColor);
                break;
            case Widget::TouchEventType::MOVED:
                _deleteButton->setTitleColor(_deleteButton->isHighlighted() ? highlightColor : normalColor);
                break;
            default:
                break;
        }
    });
    
    _deleteButton->addClickEventListener([this](Ref* sender) {
        if (!_touchEnabled) return;
        
        GameSoundManager::getInstance()->playEffectSound(touchEffect);
        
        if (_answerString.size() > 0) {
            _answerString.pop_back();
            handleAnswerStringChanged();
        }
    });

    _panelNode->addChild(_deleteButton);


    return true;
}


void AnswerPadMulti::clearAnswer()
{
    _answerString.clear();
    refreshAnswerLabels();

    _panelNode->setVisible(true);
}

void AnswerPadMulti::setAnswerDigitCount(int count)
{
    _answerDigitCount = count;

    bool showBottomRow = (_answerDigitCount > 1);
    float panelNodePosY = (showBottomRow ? 129.f : 129.f - 184.f - 10.f);

    _panelNode->setPosition(Point(_panelNode->getPosition().x, panelNodePosY));
    _zeroButton->setVisible(showBottomRow);
    _deleteButton->setVisible(showBottomRow);

    refreshAnswerLabels();
}

void AnswerPadMulti::handleAnswerStringChanged() {
    refreshAnswerLabels();

    if (_answerString.size() == _answerDigitCount) {
        if (_answerCallback)
            _answerCallback(TodoUtil::stoi(_answerString));
    }
}

void AnswerPadMulti::refreshAnswerLabels()
{
    for (size_t i = 0; i < _answerLabels.size(); ++i)
        _answerLabels[i]->removeFromParent();
    _answerLabels.clear();
 
    
    auto answerPadSize = _answerPadBg->getContentSize();

    for (size_t i = 0; i < _answerDigitCount; ++i) {
        string text = (i < _answerString.size() ? string{_answerString[i]} : "");
        bool showBottomRow = (_answerDigitCount > 1);
        float x = answerPadSize.width/2 + ((_answerDigitCount - 1) * (-.5f) + i) * 410.f;
        float y = (showBottomRow ? 1100.f : 1100.f - 92.f);

        Button* btn = ui::Button::create();
        btn->setTouchEnabled(false);
        btn->loadTextures("Counting/counting_button_answerlabel.png",
                          "Counting/counting_button_answerlabel.png");
        btn->setTitleFontName(defaultFont);
        btn->setTitleFontSize(450);
        btn->setTitleColor(Color3B(242, 245, 240));
        btn->setTitleText(text);

        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        btn->setPosition(Point(x, y));

        addChild(btn);
        _answerLabels.push_back(btn);
    }
}
