//
//  TextAndTextLayer.cpp
//  KitkitSchool
//
//  Created by timewalker on 26/12/2016.
//
//

#include "TextAndTextLayer.hpp"
#include "TextAnswerItem.hpp"

namespace
{
    const int kAnswerCount = 4;
    const float kGapBetweenEachItem = 16.f;
}

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

bool TextAndTextLayer::init()
{
    if (!Node::init()) return false;
    
    CCLOG("[TextAndImageLayer::init]");
    setContentSize(fixedDeviceSize);
    return true;
}

void TextAndTextLayer::setAnswers(std::vector<std::string> answerTexts)
{
    random_shuffle(answerTexts.begin(), answerTexts.end());
    
    auto answersRootNode = Node::create();
    
    if (answerTexts.size() > 0)
    {
        auto item = TextAnswerItem::create();
        item->initText(answerTexts[0]);
        Size totalSize = Size(item->getContentSize().width, item->getContentSize().height * kAnswerCount) + Size(0.f, kGapBetweenEachItem * (kAnswerCount - 1));
        answersRootNode->setContentSize(totalSize);
        CCLOG("total : %f, %f", totalSize.width, totalSize.height);
    }
    
    float posX = 0.f;
    float posY = answersRootNode->getContentSize().height;
    
    for (int i = 0; i < answerTexts.size(); i++)
    {
        TextAnswerItem* item = TextAnswerItem::create();
        item->initText(answerTexts[i]);
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
    
    answersRootNode->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 100.f);
    answersRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(answersRootNode);
    
    //DRAW_DEBUG_AREA(answersRootNode);
}

END_NS_LR_DIVIDED_TYPE_QUESTION;
