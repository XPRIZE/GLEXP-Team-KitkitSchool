//
//  ImageAnswerItem.cpp
//  KitkitSchool
//
//  Created by timewalker on 27/12/2016.
//
//

#include "ImageAnswerItem.hpp"
#include "Utils/TodoUtil.h"
#include "3rdParty/CCNativeAlert.h"
#include "Managers/GameSoundManager.h"

namespace ImageAnswerItemSpace
{
    std::string kLetterArray[7] = {"A", "B", "C", "D", "E", "F", "G"};
    const Size kImageSize = Size(522.f, 648.f);
    const float kImageAbsPaddingHorizontal = 35.f;
    const float kImageAbsPaddingVertical = 37.f;
    
}

using namespace ImageAnswerItemSpace;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        ImageAnswerItem::ImageAnswerItem():
        currentState(ImageAnswerState::Normal)
        {
            
        }
        
        bool ImageAnswerItem::init()
        {
            if (!Node::init())
                return false;

            return true;
        }
        
        bool ImageAnswerItem::initImage(std::string folder, std::string image, float marginX, float marginY)
        {
            _id = image;
            
            setContentSize(kImageSize);
        
            _normalAnswerSprite = Scale9Sprite::create("ComprehensionTest/MultipleChoices/comprehensivequiz-multiple-choice-normal.png");
            _normalAnswerSprite->setContentSize(kImageSize);
            _normalAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _normalAnswerSprite->setPosition(this->getContentSize() / 2);
            this->addChild(_normalAnswerSprite);
            _normalAnswerSprite->setVisible(true);
            
            _wrongAnswerSprite = Scale9Sprite::create("ComprehensionTest/MultipleChoices/comprehensivequiz-multiple-choice-wrong.png");
            _wrongAnswerSprite->setContentSize(kImageSize);
            _wrongAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _wrongAnswerSprite->setPosition(this->getContentSize() / 2);
            this->addChild(_wrongAnswerSprite);
            _wrongAnswerSprite->setVisible(false);
            
            _rightAnswerSpriteBg = Scale9Sprite::create("ComprehensionTest/MultipleChoices/comprehensivequiz-multiple-choice-picture-correct-bg.png");
            _rightAnswerSpriteBg->setContentSize(kImageSize);
            _rightAnswerSpriteBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _rightAnswerSpriteBg->setPosition(this->getContentSize() / 2);
            this->addChild(_rightAnswerSpriteBg);
            _rightAnswerSpriteBg->setVisible(false);
            
            _answerSprite = Sprite::create(folder + "/quiz/" + image);
            if (!_answerSprite) _answerSprite = Sprite::create(folder + "/page/" + image);
            if (_answerSprite == nullptr)
            {
                NativeAlert::show("Image does not exist.", image, "OK");
                return false;
            }
            float widthFactor = (kImageSize.width - kImageAbsPaddingHorizontal*2) / _answerSprite->getContentSize().width;
            float heightFactor = (kImageSize.height - kImageAbsPaddingVertical*2) / _answerSprite->getContentSize().height;
            auto scaleFactor = MIN(widthFactor, heightFactor);
            _answerSprite->setScale(scaleFactor);
            _answerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _answerSprite->setPosition(this->getContentSize() / 2);
            this->addChild(_answerSprite);
            
            _letterLabel = TodoUtil::createLabel("", 60, Size::ZERO, FONT_MUKTA_BOLD, Color4B(7, 171, 24, 255));
            _letterLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            _letterLabel->setPosition(33, this->getContentSize().height - 10.f);
            this->addChild(_letterLabel);
            
            _rightAnswerSprite = Scale9Sprite::create("ComprehensionTest/MultipleChoices/comprehensivequiz-multiple-choice-picture-correct.png", Rect(0, 0, 180, 180), Rect(126, 131, 14, 9));
            _rightAnswerSprite->setScale9Enabled(true);
            _rightAnswerSprite->setContentSize(kImageSize);
            _rightAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _rightAnswerSprite->setPosition(this->getContentSize() / 2);
            this->addChild(_rightAnswerSprite);
            _rightAnswerSprite->setVisible(false);
            
            auto *listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](Touch* T, Event* E) {
                auto P = getParent();
                auto pos = P->convertToNodeSpace(T->getLocation());
                if (this->getBoundingBox().containsPoint(pos)) {
                    CCLOG("[ImageAnswerItem::setImage] - onTouchBegan");
                    return true;
                }
                return false;
            };
            
            listener->onTouchEnded = [this](Touch* T, Event* E) {
                CCLOG("[ImageAnswerItem::setImage] - onTouchEnded");
                if (onCheckTarget) {
                    onCheckTarget();
                }
            };
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
            
            return true;
        }
        
        void ImageAnswerItem::setLetterByIndex(int index)
        {
            if (_letterLabel == nullptr)
                return;
            
            _letterLabel->setString(kLetterArray[index]);
        }
        
        std::string ImageAnswerItem::getId()
        {
            return _id;
        }
        
        bool ImageAnswerItem::isCorrect()
        {
            return true;
        }
        
        void ImageAnswerItem::setNormal()
        {
            currentState = ImageAnswerState::Normal;
            _answerSprite->setOpacity(255);
            _letterLabel->setTextColor(Color4B(7,171,24,255));
            _normalAnswerSprite->setVisible(true);
            _rightAnswerSprite->setVisible(false);
            _wrongAnswerSprite->setVisible(false);
        }
        
        void ImageAnswerItem::setRight()
        {
            currentState = ImageAnswerState::Right;
             _normalAnswerSprite->setVisible(false);
            _rightAnswerSpriteBg->setVisible(true);
            _rightAnswerSprite->setVisible(true);
            _wrongAnswerSprite->setVisible(false);
        }
        
        void ImageAnswerItem::setWrong()
        {
            GameSoundManager::getInstance()->playEffectSound("ComprehensionTest/MultipleChoices/sounds/card_miss.m4a");
            currentState = ImageAnswerState::Wrong;
            _answerSprite->setOpacity(255 * 0.2f);
            _letterLabel->setTextColor(Color4B(148,148,148,255));
            _normalAnswerSprite->setVisible(false);
            _rightAnswerSprite->setVisible(false);
            _wrongAnswerSprite->setVisible(true);
        }
        
    }
}
