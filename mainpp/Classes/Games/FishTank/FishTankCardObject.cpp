//
//  FishTankCardObject.cpp
//  todomath
//
//  Created by Jesun Kim on 2/23/16.
//
//

#include "FishTankCardObject.h"

FishTankCardObject::FishTankCardObject()
{
    m_isAnswered = false;
    m_Answer = "";
    
    m_labelLarge = NULL;
    m_labelSmall = NULL;
    m_spriteMinusLarge = NULL;
    m_spriteMinusSmall = NULL;
}

FishTankCardObject::~FishTankCardObject()
{
    
}

void FishTankCardObject::setCardTouched()
{
    m_spriteCardSmall->setVisible(false);
    
    m_spriteCardLarge->setVisible(true);
    m_spriteCardLarge->setPosition(m_spriteCardSmall->getPosition());
    m_spriteCardLarge->runAction(ScaleTo::create(0.1f, 1.0f));
    
    m_spriteCardLarge->getParent()->reorderChild(m_spriteCardLarge, 1);
}

void FishTankCardObject::setCardTouchCancel()
{
    m_spriteCardLarge->getParent()->reorderChild(m_spriteCardLarge, 0);
    
    MoveTo *move = MoveTo::create(0.1f, m_spriteCardSmall->getPosition());
    move->setTag(333);
    m_spriteCardLarge->runAction(move);
    m_spriteCardLarge->runAction(ScaleTo::create(0.1f, m_spriteCardSmall->getContentSize().width/m_spriteCardLarge->getContentSize().width));
    m_spriteCardLarge->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), Hide::create()));
    
    m_spriteCardSmall->runAction(Sequence::createWithTwoActions(DelayTime::create(0.1f), Show::create()));
}