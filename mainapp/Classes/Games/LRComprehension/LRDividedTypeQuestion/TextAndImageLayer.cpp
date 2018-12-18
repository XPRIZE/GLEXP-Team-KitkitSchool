//
//  TextAndImageLayer.cpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "TextAndImageLayer.hpp"
#include "ImageAnswerItem.hpp"

namespace
{
    const int kAnswerCount = 4;
    const float kGapBetweenEachItem = 20.f;
}

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

bool TextAndImageLayer::init()
{
    if (!Node::init()) return false;
    
    CCLOG("[TextAndImageLayer::init]");
    setContentSize(fixedDeviceSize);
    return true;
}

void TextAndImageLayer::setAnswers(std::vector<std::string> imageFiles)
{
    random_shuffle(imageFiles.begin(), imageFiles.end());
    
    auto answersRootNode = Node::create();
    
    if (imageFiles.size() > 0)
    {
        auto item = ImageAnswerItem::create();
        item->initImage(imageFiles[0]);
        Size totalSize = Size(item->getContentSize().width * kAnswerCount, item->getContentSize().height) + Size(kGapBetweenEachItem * (kAnswerCount - 1), 0.f);
        answersRootNode->setContentSize(totalSize);
    }
    
    float posX = 0.f;
    
    for (int i = 0; i < imageFiles.size(); i++)
    {
        ImageAnswerItem* item = ImageAnswerItem::create();
        bool bInit = item->initImage(imageFiles[i]);
        if (bInit) item->setLetterByIndex(i);
        
        item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        item->setPosition(posX, item->getContentSize().height / 2);
        answersRootNode->addChild(item);
        
        posX += item->getContentSize().width + kGapBetweenEachItem;
        
        item->onCheckTarget = [this, item]() {
            if(item->currentState == ImageAnswerState::Right || item->currentState == ImageAnswerState::Wrong)
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
    
    answersRootNode->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 100.f);
    answersRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(answersRootNode);
}

END_NS_LR_DIVIDED_TYPE_QUESTION;
