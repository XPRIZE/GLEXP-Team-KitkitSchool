//
//  FishTankSlotObject.h
//  todomath
//
//  Created by Jesun Kim on 2/25/16.
//
//

#ifndef FishTankSlotObject_h
#define FishTankSlotObject_h

#include "cocos2d.h"

USING_NS_CC;

class FishTankSlotObject : public Ref {
    
public:
    FishTankSlotObject();
    ~FishTankSlotObject();
    
public:
    Sprite *m_spriteSlot;
    
    std::string m_arrAnswer[2];
    
    bool m_isAnswered;
};

#endif /* FishTankSlotObject_h */
