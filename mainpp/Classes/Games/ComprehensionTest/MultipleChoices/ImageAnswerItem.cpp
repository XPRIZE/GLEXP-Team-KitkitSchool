//
//  ImageAnswerItem.cpp
//  enumaXprize
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
    const Size kImageSize = Size(510.f, 394.f);
    const float kImageAbsPadding = 22.f;
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
        
        void ImageAnswerItem::initImage(std::string imagePath, float marginX, float marginY)
        {
            _id = imagePath;
            
            setContentSize(kImageSize);
            
            _background = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_answers_2.png");
            _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            setContentSize(Size(_background->getContentSize().width + marginX, _background->getContentSize().height + marginY));
            _background->setPosition(getContentSize() / 2);
            addChild(_background);
            
            auto image = Sprite::create("ComprehensionTest/Image/" + imagePath);
            if (image == nullptr) { NativeAlert::show("Image does not exist.", "ComprehensionTest/Image/" + imagePath, "OK"); return; }
            
            float widthFactor = (kImageSize.width - kImageAbsPadding) / image->getContentSize().width;
            float heightFactor = (kImageSize.height - kImageAbsPadding) / image->getContentSize().height;
            auto scaleFactor = MIN(widthFactor, heightFactor);
            image->setScale(scaleFactor);
            image->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            image->setPosition(getContentSize() / 2);
            addChild(image);
            
            _normalAnswerSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_answers_1.png");
            _normalAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _normalAnswerSprite->setPosition(getContentSize() / 2);
            addChild(_normalAnswerSprite);
            _normalAnswerSprite->setVisible(true);
            
            _letterLabel = TodoUtil::createLabel("", 60, Size::ZERO, "fonts/TodoSchoolV2.ttf", Color4B::BLACK);
            _letterLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            _letterLabel->setPosition(35.f, _normalAnswerSprite->getContentSize().height - 15.f);
            _normalAnswerSprite->addChild(_letterLabel);
            
            _rightAnswerSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_answer_right_1.png");
            _rightAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _rightAnswerSprite->setPosition(getContentSize() / 2);
            addChild(_rightAnswerSprite);
            _rightAnswerSprite->setVisible(false);
            
            _wrongAnswerSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_answer_wrong.png");
            _wrongAnswerSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            _wrongAnswerSprite->setPosition(getContentSize() / 2);
            addChild(_wrongAnswerSprite);
            _wrongAnswerSprite->setVisible(false);
            
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
            _normalAnswerSprite->setVisible(true);
            _rightAnswerSprite->setVisible(false);
            _wrongAnswerSprite->setVisible(false);
        }
        
        void ImageAnswerItem::setRight()
        {
            currentState = ImageAnswerState::Right;
             _normalAnswerSprite->setVisible(false);
            _rightAnswerSprite->setVisible(true);
            _wrongAnswerSprite->setVisible(false);
        }
        
        void ImageAnswerItem::setWrong()
        {
            GameSoundManager::getInstance()->playEffectSound("ComprehensionTest/MultipleChoices/sounds/card_miss.m4a");
            currentState = ImageAnswerState::Wrong;
            _normalAnswerSprite->setVisible(false);
            _rightAnswerSprite->setVisible(false);
            _wrongAnswerSprite->setVisible(true);
        }
        
    }
}
