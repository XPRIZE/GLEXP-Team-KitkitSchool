//
//  ImageAndImageLayer.cpp
//  KitkitSchool
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
        
        void ImageAndImageLayer::setQuestionImage(std::string folder, std::string imageFile)
        {
            auto backgroundSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_img.png");
            backgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            backgroundSprite->setPosition(getContentSize().width / 2 + kRevisedX, getContentSize().height / 2 + kRevisedY);
            addChild(backgroundSprite);
            
            auto questionSprite = Sprite::create(folder + "/quiz/" + imageFile);
            if (!questionSprite) questionSprite = Sprite::create(folder + "/page/" + imageFile);
            if (questionSprite == nullptr) { NativeAlert::show("Image does not exist.", imageFile, "OK"); return; }
            
            questionSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            questionSprite->setPosition(backgroundSprite->getContentSize() / 2);
            backgroundSprite->addChild(questionSprite);
            
            auto frameSprite = Sprite::create("ComprehensionTest/MultipleChoices/comprehention_multiplechoice_img_border.png");
            frameSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            frameSprite->setPosition(backgroundSprite->getContentSize() / 2);
            backgroundSprite->addChild(frameSprite);
        }
        
        void ImageAndImageLayer::setAnswers(std::string folder, std::vector<std::string> imageFiles)
        {
            Size totalSize;
            auto answersRootNode = Node::create();
            
            if (imageFiles.size() > 0)
            {
                auto item = ImageAnswerItem::create();
                item->initImage(folder, imageFiles[0], 30.f, 50.f);
                int lineNumbers = imageFiles.size() / kMaxNumbersPerLine;
                totalSize = Size(item->getContentSize().width * kMaxNumbersPerLine, item->getContentSize().height * lineNumbers);
                answersRootNode->setContentSize(totalSize);
            }
            
            CCLOG("total : %f, %f", totalSize.width, totalSize.height);
            
            for (int i = 0; i < imageFiles.size(); i++)
            {
                ImageAnswerItem* item = ImageAnswerItem::create();
                bool bInit = item->initImage(folder, imageFiles[i], 30.f, 50.f);
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
