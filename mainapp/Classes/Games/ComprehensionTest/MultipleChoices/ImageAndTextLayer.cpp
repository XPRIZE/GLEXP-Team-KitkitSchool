//
//  ImageAndTextLayer.cpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#include "ImageAndTextLayer.hpp"
#include "TextAnswerItem.hpp"
#include "3rdParty/CCNativeAlert.h"

namespace ImageAndTextLayerSpace
{
    const float kRevisedX = -100.f;
    const float kRevisedY = -200.f;
}

using namespace ImageAndTextLayerSpace;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        bool ImageAndTextLayer::init()
        {
            if (!Node::init()) return false;
            
            CCLOG("[ImageAndTextLayer::init]");
            setContentSize(fixedDeviceSize);
            return true;
        }
        
        void ImageAndTextLayer::setQuestionImage(std::string imagePath)
        {
            auto backgroundSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_img.png");
            backgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            backgroundSprite->setPosition(getContentSize().width / 2 + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(backgroundSprite);
            
            auto questionSprite = Sprite::create("ComprehensionTest/Image/" + imagePath);
            if (questionSprite == nullptr) { NativeAlert::show("Image does not exist.", "ComprehensionTest/Image/" + imagePath, "OK"); return; }
            
            questionSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            questionSprite->setPosition(backgroundSprite->getContentSize() / 2);
            backgroundSprite->addChild(questionSprite);
            
            auto frameSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_img_border.png");
            frameSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            frameSprite->setPosition(backgroundSprite->getContentSize() / 2);
            backgroundSprite->addChild(frameSprite);
        }
        
        void ImageAndTextLayer::setAnswers(std::vector<std::string> answerTexts)
        {
            Size totalSize;
            auto answersRootNode = Node::create();
            
            if (answerTexts.size() > 0)
            {
                auto item = TextAnswerItem::create();
                item->initText(answerTexts[0]);
                totalSize = Size(item->getContentSize().width, item->getContentSize().height * 4);
                answersRootNode->setContentSize(totalSize);
            }
            
            CCLOG("total : %f, %f", totalSize.width, totalSize.height);
            
            for (int i = 0; i < answerTexts.size(); i++)
            {
                TextAnswerItem* item = TextAnswerItem::create();
                item->initText(answerTexts[i], false);
                item->setLetterByIndex(i);
                
                float posX = 0;
                float posY = answersRootNode->getContentSize().height - item->getContentSize().height * i;
                
                CCLOG("(%f, %f)", posX, posY);
                
                item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                item->setPosition(posX, posY);
                answersRootNode->addChild(item);
                
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
            
            answersRootNode->setPosition(getContentSize().width / 2 - 90.f, getContentSize().height / 2 - 100.f);
            answersRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            addChild(answersRootNode);
        }
    }
}
