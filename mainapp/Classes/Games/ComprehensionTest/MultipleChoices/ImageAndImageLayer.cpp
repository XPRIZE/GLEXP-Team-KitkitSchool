//
//  ImageAndImageLayer.cpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#include "ImageAndImageLayer.hpp"
#include "ImageAnswerItem.hpp"
#include "3rdParty/CCNativeAlert.h"

namespace ImageAndImageLayerSpace
{
    int kMaxNumbersPerLine = 2;
    float kRevisedX = -100.f;
    float kRevisedY = -200.f;
}

using namespace ImageAndImageLayerSpace;

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        bool ImageAndImageLayer::init()
        {
            if (!Node::init()) return false;
            
            CCLOG("[ImageAndImageLayer::init]");
            setContentSize(fixedDeviceSize);
            return true;
        }
        
        void ImageAndImageLayer::setQuestionImage(std::string imagePath)
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
        
        void ImageAndImageLayer::setAnswers(std::vector<std::string> imagePaths)
        {
            Size totalSize;
            auto answersRootNode = Node::create();
            
            if (imagePaths.size() > 0)
            {
                auto item = ImageAnswerItem::create();
                item->initImage(imagePaths[0], 30.f, 50.f);
                int lineNumbers = imagePaths.size() / kMaxNumbersPerLine;
                totalSize = Size(item->getContentSize().width * kMaxNumbersPerLine, item->getContentSize().height * lineNumbers);
                answersRootNode->setContentSize(totalSize);
            }
            
            CCLOG("total : %f, %f", totalSize.width, totalSize.height);
            
            for (int i = 0; i < imagePaths.size(); i++)
            {
                ImageAnswerItem* item = ImageAnswerItem::create();
                bool bInit = item->initImage(imagePaths[i], 30.f, 50.f);
                if (bInit) item->setLetterByIndex(i);
                
                float posX = item->getContentSize().width * (i % kMaxNumbersPerLine);
                float posY = totalSize.height - item->getContentSize().height * (i / kMaxNumbersPerLine);
                CCLOG("%f, %f", posX, posY);
                
                item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                item->setPosition(posX, posY);
                answersRootNode->addChild(item);
                
                item->onCheckTarget = [this, item]() {
                    if(item->currentState == ImageAnswerState::Wrong)
                        return;
                    
                    if (parentScene->isCorrect(item->getId()))
                    {
                        parentScene->solve(item->getId());
                        item->setRight();
                        
                        if (parentScene->isSolved())
                            parentScene->onSolve();
                    }
                    else
                    {
                        item->setWrong();
                    }
                };
            }
            
            answersRootNode->setPosition(getContentSize().width / 2 - 100.f, getContentSize().height / 2 - 100.f);
            answersRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            addChild(answersRootNode);
        }
    }
}
