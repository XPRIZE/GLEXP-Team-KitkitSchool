//
//  FishTankCardObject.h
//  todomath
//
//  Created by Jesun Kim on 2/23/16.
//
//

#ifndef FishTankCardObject_h
#define FishTankCardObject_h

#include "cocos2d.h"

#include "ui/CocosGUI.h"

#include "FishTankSlotObject.h"

USING_NS_CC;
using namespace ui;

class FishTankCardObject : public Ref {
    
public:
    FishTankCardObject();
    ~FishTankCardObject();
    
    void setCardTouched();
    void setCardTouchCancel();
    
public:
    Sprite *m_spriteCardSmall;
    Sprite *m_spriteCardSmallShadow;
    Sprite *m_spriteMinusSmall;
    
    Sprite *m_spriteCardLarge;
    Sprite *m_spriteCardLargeDepth;
    Sprite *m_spriteMinusLarge;
    
    Label *m_labelSmall;
    Label *m_labelLarge;
    
//    Text *m_textSmall;
//    Text *m_textLarge;
    
    std::string m_Answer;
    
    bool m_isAnswered;
    
    FishTankSlotObject *m_mySlot;
};

#endif /* FishTankCardObject_h */
