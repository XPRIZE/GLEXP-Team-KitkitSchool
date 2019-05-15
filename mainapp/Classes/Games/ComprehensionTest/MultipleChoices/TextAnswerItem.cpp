//
//  TextAnswerItem.cpp
//  KitkitSchool
//
//  Created by timewalker on 27/12/2016.
//
//

#include "TextAnswerItem.hpp"
#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"

namespace TextAnswerItemSpace
{
    const std::string kPrefixPath = "ComprehensionTest/MultipleChoices/";
    
    const std::string kNormalAnswerSpritePath = kPrefixPath + "comprehensivequiz_multiple_choice_text.png";
    const std::string kRightAnswerSpritePath = kPrefixPath + "comprehensivequiz_multiple_choice_text_right.png";
    const std::string kWrongAnswerSpritePath = kPrefixPath + "comprehensivequiz_multiple_choice_text_wrong.png";
    
    const std::string kLongNormalAnswerSpritePath = kPrefixPath + "comprehensivequiz_multiple_choice_longtext.png";
    const std::string kLongRightAnswerSpritePath = kPrefixPath + "comprehensivequiz_multiple_choice_longtext_right.png";
    const std::string kLongWrongAnswerSpritePath = kPrefixPath + "comprehensivequiz_multiple_choice_longtext_wrong.png";
    
    const std::string kCheckSpritePath = kPrefixPath + "comprehensivequiz_correct_check.png";
    
    const Color4B kNormalAnswerColor = Color4B(56, 56, 56, 255);
    const Color4B kRightAnswerColor = Color4B(255, 255, 255, 255);
    const Color4B kWrongAnswerColor = Color4B(148, 148, 148, 255);
    
    const Color4B kNormalLetterColor = Color4B(7, 171, 24, 255);
    const Color4B kWrongLetterColor = Color4B(148, 148, 148, 255);
    
    const Size kDeaultContentSize = Size(1196.f, 180.f);
    const Size kLongContentSize = Size(2036.f, 180.f);
    
    const std::string kLetterArray[7] = {"A", "B", "C", "D", "E", "F", "G"};
}

using namespace TextAnswerItemSpace;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        const float kRevisedX = 10.f;
        const float kRevisedY = 10.f;
        
        TextAnswerItem::TextAnswerItem():
        currentState(TextAnswerState::Normal)
        {

        }
        
        bool TextAnswerItem::init()
        {
            if (!Node::init())
                return false;
            
            return true;
        }
        
        void TextAnswerItem::initText(std::string text, bool bLongText)
        {
            _id = text;
            
            setContentSize(bLongText ? kLongContentSize : kDeaultContentSize);
            
            _normalAnswerSprite = Sprite::create(bLongText ? kLongNormalAnswerSpritePath : kNormalAnswerSpritePath);
            _normalAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _normalAnswerSprite->setPosition(0.f, getContentSize().height / 2);
            _normalAnswerSprite->setVisible(true);
            addChild(_normalAnswerSprite);

            _rightAnswerSprite = Sprite::create(bLongText ? kLongRightAnswerSpritePath : kRightAnswerSpritePath);
            _rightAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _rightAnswerSprite->setPosition(0.f, getContentSize().height / 2);
            _rightAnswerSprite->setVisible(false);
            addChild(_rightAnswerSprite);
            
            _wrongAnswerSprite = Sprite::create(bLongText ? kLongWrongAnswerSpritePath : kWrongAnswerSpritePath);
            _wrongAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _wrongAnswerSprite->setPosition(0.f, getContentSize().height / 2);
            _wrongAnswerSprite->setVisible(false);
            addChild(_wrongAnswerSprite);
            
            _normalLetterLabel = TodoUtil::createLabel("", 60, Size::ZERO, FONT_ANDIKA_REGULAR, kNormalLetterColor);
            _normalLetterLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _normalLetterLabel->setPosition(50.f + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_normalLetterLabel);
            
            _wrongLetterLabel = TodoUtil::createLabel("", 60, Size::ZERO, FONT_ANDIKA_REGULAR, kWrongLetterColor);
            _wrongLetterLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _wrongLetterLabel->setPosition(50.f + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_wrongLetterLabel);
            
            _normalAnswerLabel = TodoUtil::createLabelMultilineToFit(text, 60, Size(_normalAnswerSprite->getContentSize().width - 210.f, 160.f),  FONT_ANDIKA_REGULAR, kNormalAnswerColor);
            _normalAnswerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _normalAnswerLabel->setPosition(150.f + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_normalAnswerLabel);

            _rightAnswerLabel = TodoUtil::createLabelMultilineToFit(text, 60, Size(_rightAnswerSprite->getContentSize().width - 210.f, 160.f),  FONT_ANDIKA_REGULAR, kRightAnswerColor);
            _rightAnswerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _rightAnswerLabel->setPosition(150.f + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_rightAnswerLabel);
            
            _wrongAnswerLabel = TodoUtil::createLabelMultilineToFit(text, 60, Size(_wrongAnswerSprite->getContentSize().width - 210.f, 160.f),  FONT_ANDIKA_REGULAR, kWrongAnswerColor);
            _wrongAnswerLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _wrongAnswerLabel->setPosition(150.f + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_wrongAnswerLabel);
            
            _checkSprite = Sprite::create(kCheckSpritePath);
            _checkSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            _checkSprite->setPosition(50.f + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_checkSprite);
            
            setState(TextAnswerState::Normal);
            
            auto *listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](Touch* T, Event* E) {
                auto P = getParent();
                auto pos = P->convertToNodeSpace(T->getLocation());
                if (this->getBoundingBox().containsPoint(pos)) {
                    CCLOG("[TextAnswerItem::setImage] - onTouchBegan");
                    return true;
                }
                return false;
            };
            
            listener->onTouchEnded = [this](Touch* T, Event* E) {
                CCLOG("[TextAnswerItem::setImage] - onTouchEnded");
                if (onCheckTarget) {
                    onCheckTarget();
                }
            };
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        }
        
        void TextAnswerItem::setLetterByIndex(int index)
        {
            _normalLetterLabel->setString(kLetterArray[index]);
            _wrongLetterLabel->setString(kLetterArray[index]);
        }
        
        std::string TextAnswerItem::getId()
        {
            return _id;
        }
        
        bool TextAnswerItem::isCorrect()
        {
            return true;
        }
        
        
        void TextAnswerItem::setState(TextAnswerState state)
        {
            currentState = state;

            if (state == TextAnswerState::Wrong)
            {
                GameSoundManager::getInstance()->playEffectSound("ComprehensionTest/MultipleChoices/sounds/card_miss.m4a");
            }
            
            _normalAnswerSprite->setVisible(state == TextAnswerState::Normal ? true : false);
            _normalAnswerLabel->setVisible(state == TextAnswerState::Normal ? true : false);
            
            _rightAnswerSprite->setVisible(state == TextAnswerState::Right ? true : false);
            _rightAnswerLabel->setVisible(state == TextAnswerState::Right ? true : false);
            
            _wrongAnswerSprite->setVisible(state == TextAnswerState::Wrong ? true : false);
            _wrongAnswerLabel->setVisible(state == TextAnswerState::Wrong ? true : false);
            
            _normalLetterLabel->setVisible(state == TextAnswerState::Normal ? true : false);
            _checkSprite->setVisible(state == TextAnswerState::Right ? true : false);
            _wrongLetterLabel->setVisible(state == TextAnswerState::Wrong ? true : false);
        }
    }
}
