//
//  FishTankTankObject.h
//  todomath
//
//  Created by Jesun Kim on 2/19/16.
//
//

#ifndef FishTankTankObject_h
#define FishTankTankObject_h

#include "cocos2d.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocostudio::timeline;
using namespace ui;

class FishTankTankObject : public Ref {
    
public:
    FishTankTankObject(std::string strFileName);
    ~FishTankTankObject();
    
    void setBoundary();
    void setFishCount(int number);
    void initData();
    void drawZeroAnimation();
    void stopZeroAnimation();
    void setMinusFishCount();
    
public:
    Node *m_node;
    
    Sprite *m_spriteCircle;
    
    Text *m_text;
    Text *m_textPhase2;
    
    int m_leftBoundary;
    int m_rightBoundary;
    int m_topBoundary;
    int m_bottomBoundary;
    
    int m_leftEnter;
    int m_rightEnter;
    int m_topEnter;
    int m_bottomEnter;
    
    bool m_isSource;
    bool m_isClear;
    
    int m_fishFirstCount;
    int m_fishCount;
    int m_needCount;
private:
    const char* defaultFont = "fonts/TodoSchoolV2.ttf";

};

#endif /* FishTankTankObject_h */
