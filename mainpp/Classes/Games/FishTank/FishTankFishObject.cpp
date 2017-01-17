//
//  FishTankFishObject.cpp
//  todomath
//
//  Created by Jesun Kim on 2/17/16.
//
//

#include "FishTankFishObject.h"

FishTankFishObject::FishTankFishObject()
{
    m_isMoving = true;
    this->setChangeMovingSpeedCount();
    this->setMovingSpeed();
    this->setForceDirectionCount();
}

FishTankFishObject::~FishTankFishObject()
{
}

// 물고기의 이동 속도
void FishTankFishObject::setMovingSpeed()
{
    m_movingSpeed = RandomHelper::random_real(0.7f, 2.5f);
}

// m_changeMovingSpeedCount = 0 이면 물고기의 이동속도 변경
void FishTankFishObject::setChangeMovingSpeedCount()
{
    m_changeMovingSpeedCount = RandomHelper::random_int(30, 50);
}

// m_forceDirectionCount = 0 이면 강제 방향 전환
void FishTankFishObject::setForceDirectionCount()
{
    m_forceDirectionCount = RandomHelper::random_int(30, 50);
}

// 물고기 방향 전환
void FishTankFishObject::setDirection()
{
    if (!m_isLeft) {
        m_node->setScaleX(-1);
    }else{
        m_node->setScaleX(1);
    }
}

// 물고기 터치 영역 체크
Rect FishTankFishObject::getFishBoundingBox()
{
    //Sprite *sprite = static_cast<Sprite *>(m_node->getChildByName("sprite"));
    Sprite *sprite = static_cast<Sprite *>(m_node);
    
    //Vec2 position = m_node->convertToWorldSpace(sprite->getPosition());
    Vec2 position = sprite->convertToWorldSpace(Point(sprite->getContentSize()/2));
    return Rect(position.x - std::abs(sprite->getContentSize().width/2*sprite->getScaleX()),
                position.y - std::abs(sprite->getContentSize().height/2*sprite->getScaleY()),
                sprite->getContentSize().width*std::abs(sprite->getScaleX()),
                sprite->getContentSize().height*std::abs(sprite->getScaleY()));
}
