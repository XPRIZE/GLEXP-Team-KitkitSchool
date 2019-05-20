//
//  EquationMakerObject.h
//  todomath
//
//  Created by Hye Kyoung Hong on 7/23/15.
//
//

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string.h>

#include "EquationMakerScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;

class EquationMakerObject : public Node
{
public:
    enum{
        K_ORDER_SHADOW = 0,
        K_ORDER_LINE,
        K_ORDER_CARD,
        K_ORDER_OBJECT,
    };

public:
	EquationMakerObject();
	~EquationMakerObject();

	CREATE_FUNC(EquationMakerObject);

	virtual bool init(); 
	virtual void onEnter();
	virtual void onExit();
    
    
public:
    bool isSlotIn();
    bool isTouchedIn(Point pos);

    void setTopLayer(EquationMakerScene* pLayer);
    void createObject(int type, int rowType, int levelType,
                      std::string name, Size size, bool isSlotIn, bool isSlotHide);
    
    void setObjectCard(int num, int levelType, std::string name);
    void setObjectAndNumberCard(int num, int levelType, std::string name);
    
    void setSlotIn(bool isSlotIn);
    void setShadowVisible(bool isVisible);
    void setOriginPos(Point pos);
    void setHintObject();

    
    int getType();
    int getRowType();
    
    Point getOriginPos();
    
    std::string getFileName(int levelType, std::string name, int num = 0);
    
public:
    Sprite* m_pCardSprite;
    Sprite* m_pLineSprite;
    Sprite* m_pShadowSprite;
    
    EquationMakerScene* m_pTopLayer;
    
    Point m_OriginPos;

private:
    int m_Type;
    int m_RowType;
    int m_LevelType;
    
    bool m_isSlotIn;
    bool m_isSlotHide;
    bool m_isHintObject;
    
    const char* defaultFont = "fonts/mukta-bold.ttf";
    
    
};
