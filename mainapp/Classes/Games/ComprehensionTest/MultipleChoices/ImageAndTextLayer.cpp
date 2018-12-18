//
//  ImageAndTextLayer.cpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "ImageAndTextLayer.hpp"
#include "TextAnswerItem.hpp"
#include "3rdParty/CCNativeAlert.h"

namespace ImageAndTextLayerSpace
{
    const float kRevisedX = -210.f;
    const float kRevisedY = -360.f;
}

using namespace ImageAndTextLayerSpace;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        const int kAnswerCount = 4;
        const float kGapBetweenEachItem = 18.f;
        const float kPadding = 38.f;
        
        bool ImageAndTextLayer::init()
        {
            if (!Node::init()) return false;
            
            CCLOG("[ImageAndTextLayer::init]");
            setContentSize(fixedDeviceSize);
            return true;
        }
        
        void ImageAndTextLayer::setQuestionImage(std::string folder, std::string imageFile)
        {
            _backgroundSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_img.png");
            _backgroundSprite->setContentSize(Size(852, 876));
            _backgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _backgroundSprite->setPosition(getContentSize().width / 2 + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(_backgroundSprite);
            
            auto questionSprite = Sprite::create(folder + "/quiz/" + imageFile);
            if (!questionSprite) questionSprite = Sprite::create(folder + "/page/" + imageFile);
            if (questionSprite == nullptr) { NativeAlert::show("Image does not exist.", imageFile, "OK"); return; }
            
            questionSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            questionSprite->setPosition(_backgroundSprite->getContentSize() / 2);
            float minScale = MIN((_backgroundSprite->getContentSize().width - kPadding * 2) / questionSprite->getContentSize().width, (_backgroundSprite->getContentSize().height - kPadding * 2) / questionSprite->getContentSize().height);
            questionSprite->setScale(minScale);
            _backgroundSprite->addChild(questionSprite);
            
//            auto frameSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_img_border.png");
//            frameSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//            frameSprite->setPosition(_backgroundSprite->getContentSize() / 2);
//            _backgroundSprite->addChild(frameSprite);
        }
        
        void ImageAndTextLayer::setAnswers(std::vector<std::string> answerTexts)
        {
            random_shuffle(answerTexts.begin(), answerTexts.end());
            
            auto answersRootNode = Node::create();
            
            if (answerTexts.size() > 0)
            {
                auto item = TextAnswerItem::create();
                item->initText(answerTexts[0]);
                Size totalSize = Size(item->getContentSize().width, item->getContentSize().height * kAnswerCount) + Size(0.f, kGapBetweenEachItem * (kAnswerCount - 1));
                answersRootNode->setContentSize(totalSize);
            }
            
            float posX = 0.f;
            float posY = answersRootNode->getContentSize().height;
            
            for (int i = 0; i < answerTexts.size(); i++)
            {
                TextAnswerItem* item = TextAnswerItem::create();
                item->initText(answerTexts[i], false);
                item->setLetterByIndex(i);                
                item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                item->setPosition(posX, posY);
                answersRootNode->addChild(item);
                
                posY -= item->getContentSize().height + kGapBetweenEachItem;
                
                item->onCheckTarget = [this, item]() {
                    if(item->currentState == TextAnswerState::Right || item->currentState == TextAnswerState::Wrong)
                        return;
                    
                    if (parentScene->isCorrect(item->getId()))
                    {
                        parentScene->solve(item->getId());
                        item->setState(TextAnswerState::Right);
                        
                        if (parentScene->isSolved())
                            parentScene->onSolve();
                    }
                    else
                    {
                        item->setState(TextAnswerState::Wrong);
                    }
                };
            }
            
            //            LayerColor* lc1 = LayerColor::create(Color4B(100, 100, 100, 255));
            //            lc1->setContentSize(answersRootNode->getContentSize());
            //            lc1->ignoreAnchorPointForPosition(false);
            //            lc1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            //            lc1->setPosition(answersRootNode->getContentSize() / 2);
            //            answersRootNode->addChild(lc1);
            
            answersRootNode->setPosition(getContentSize().width / 2 - 150.f, _backgroundSprite->getPosition().y);
            answersRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            addChild(answersRootNode);
        }
    }
}
