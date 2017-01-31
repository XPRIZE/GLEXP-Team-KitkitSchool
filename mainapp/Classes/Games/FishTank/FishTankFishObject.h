//
//  FishTankFishObject.h
//  todomath
//
//  Created by Jesun Kim on 2/17/16.
//
//

#ifndef FishTankFishObject_h
#define FishTankFishObject_h

#include "cocos2d.h"

#include "FishTankTankObject.h"

USING_NS_CC;

class FishTankFishObject : public Ref {
    
public:
    FishTankFishObject();
    ~FishTankFishObject();
    
    void setDirection();
    void setMovingSpeed();
    void setForceDirectionCount();
    void setChangeMovingSpeedCount();
    Rect getFishBoundingBox();
    
public:
    Node *m_node;
    
    FishTankTankObject *m_myTank;
    
    Vec2 m_prevPosition;
    
    bool m_isLeft;
    bool m_isMoving;
    
    float m_movingSpeed;
    
    int m_changeMovingSpeedCount;
    int m_forceDirectionCount;
};

#endif /* FishTankFishObject_h */
