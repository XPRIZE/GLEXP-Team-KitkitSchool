//
//  TextAndTextLayer.cpp
//  enumaXprize
//
//  Created by timewalker on 26/12/2016.
//
//

#include "TextAndTextLayer.hpp"
#include "TextAnswerItem.hpp"

namespace ComprehensionTest
{
    namespace MultipleChoices
    {
        bool TextAndTextLayer::init()
        {
            if (!Node::init()) return false;
            
            CCLOG("[TextAndImageLayer::init]");
            setContentSize(fixedDeviceSize);
            return true;
        }
        
        void TextAndTextLayer::setAnswers(std::vector<std::string> answerTexts)
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
                item->initText(answerTexts[i]);
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
            
            answersRootNode->setPosition(getContentSize().width / 2, getContentSize().height / 2 - 100.f);
            answersRootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            addChild(answersRootNode);
        }
        
    }
}
