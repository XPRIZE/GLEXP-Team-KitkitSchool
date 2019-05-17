#include "EquationMakerObject.h"
#include "EquationMakerScene.h"
#include "Utils/TodoUtil.h"

EquationMakerObject::EquationMakerObject(){
    m_Type = 0;
    
    m_isSlotIn = false;
    m_isSlotHide = false;
    m_isHintObject = false;
    
    m_pCardSprite = NULL;
    m_pLineSprite = NULL;
    m_pShadowSprite = NULL;
    
    m_pTopLayer = NULL;
}

EquationMakerObject::~EquationMakerObject(){

}

bool EquationMakerObject::init(){
	return true;
}

void EquationMakerObject::onEnter(){
	Node::onEnter();
}

void EquationMakerObject::setHintObject(){
    m_isHintObject = true;
}

void EquationMakerObject::createObject(int type, int rowType, int levelType, std::string name, Size size, bool isSlotIn, bool isSlotHide){
    m_Type = type;
    m_isSlotIn = isSlotIn;
    m_isSlotHide = isSlotHide;
    m_RowType = rowType;
    m_LevelType = levelType;
    
    TextHAlignment hAlign = TextHAlignment::CENTER;
    
    float fontSize = 220;
    auto color = Color3B(255, 255, 255);
    if(isSlotHide == false){
        std::string levelStr = (levelType == 5) ? "_5level" :"";
        
        std::string cardName = "EquationMaker/block_01"+levelStr+".png";
        std::string lineName = "EquationMaker/block_02"+levelStr+".png";
        std::string shadowName = "EquationMaker/block_03_shadow"+levelStr+".png";

        if(isSlotIn == false && m_isHintObject == false){
            m_pShadowSprite = Sprite::create(shadowName.c_str());
            m_pShadowSprite->setPositionX(4.f);
            this->addChild(m_pShadowSprite, K_ORDER_SHADOW);
            
            m_pLineSprite = Sprite::create(lineName.c_str());
            m_pLineSprite->setPositionX(4.f);
            this->addChild(m_pLineSprite, K_ORDER_LINE);
        }

        m_pCardSprite = Sprite::create(cardName.c_str());
        this->addChild(m_pCardSprite, K_ORDER_CARD);
        
        color = Color3B(165, 96, 83);
        fontSize = 200.f;
    }
    
    if((rowType == EquationMakerScene::K_OBJECT_TYPE_NUMBER) ||
       (rowType == EquationMakerScene::K_OBJECT_TYPE_OBJECT_NUMBER) ||
       (type >= EquationMakerScene::K_TYPE_SIGN_PLUS)){
        float addX = 0.f;
        float addY = 0.f;
        std::string str = "";
        if(type == EquationMakerScene::K_TYPE_SIGN_PLUS) str = "+";
        else if(type == EquationMakerScene::K_TYPE_SIGN_MINUS) str = "-";
        else if(type == EquationMakerScene::K_TYPE_SIGN_EQ) str = "=";
        else{
            char buf[16];
            sprintf(buf, "%d", type);
            str = buf;
            
            if(strstr(buf, "1")) addX = -20.f;
            
            if(rowType == EquationMakerScene::K_OBJECT_TYPE_OBJECT_NUMBER){
                fontSize = 160.f;
                addY = -(m_pCardSprite->getContentSize().height*0.30f);
            }
        }
        
        if(levelType == 5 && type >= EquationMakerScene::K_TYPE_SIGN_PLUS){
            addX = -100.f;
            fontSize = 100.f;
            
            hAlign = TextHAlignment::LEFT;
        }

        auto text = TodoUtil::createLabel(str, fontSize, Size::ZERO, defaultFont, Color4B::WHITE);
        text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        text->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
        text->setColor(color);
        text->setPosition(addX, addY);
        this->addChild(text, K_ORDER_OBJECT);
    }
    else if(rowType == EquationMakerScene::K_OBJECT_TYPE_OBJECT){
        setObjectCard(type, levelType, name);
    }
    
    if((rowType == EquationMakerScene::K_OBJECT_TYPE_OBJECT_NUMBER) &&
       (type < EquationMakerScene::K_TYPE_SIGN_PLUS)){
        setObjectAndNumberCard(type, levelType, name);
    }
}

void EquationMakerObject::setObjectAndNumberCard(int num, int levelType,std::string name){
    std::vector<Point> vecPosList;
    std::string fileName = getFileName(levelType, name, num);
    
    auto sprite = Sprite::create(fileName.c_str());
    if(sprite){
        float startX = 0.f;
        float startY = m_pCardSprite->getContentSize().height*0.15f;
        
        Vec2 scale = Vec2(1.f, 1.f);
        if(num <= 10){
            
            
            scale = Vec2(0.8, 0.8);

            
            
            Size size = sprite->getContentSize();
            
            
            size.width *= scale.x;
            size.height *= scale.y;
            
            size.width *= 1.2f;
            size.height *= 1.1f;
            
            
            int height = 0;
            int addValue = 0;
            if(num >= 1 && num <= 5) height = num;
            else{
                addValue = (num%2 > 0) ? 1:0;
                height = (num/2)+addValue;
            }
            
            startY = m_pTopLayer->getStartPoint(height, startY, size.height);
            for(int i = 0; i < height; ++i){
                int col = (num/height)+addValue;
                int width = MIN(num-(i*col), col);
                
                startX = m_pTopLayer->getStartPoint(width, 0.f, size.width);
                for(int j = 0; j < width; ++j){
                    vecPosList.push_back(Point(startX, startY));
                    startX += size.width;
                }
                
                startY += size.height;
            }
        }
        else{
            scale = Vec2(0.8, 0.8);
            
            vecPosList.push_back(Point(startX, startY));
        }
        
        for(unsigned int i = 0; i < vecPosList.size(); ++i){
            auto pObject = Sprite::create(fileName.c_str());
            if(pObject){
                pObject->setPosition(vecPosList[i]);
                pObject->setScale(scale.x, scale.y);
                this->addChild(pObject, K_ORDER_OBJECT);
            }
        }

    }
}


void EquationMakerObject::setObjectCard(int num, int levelType, std::string name){
    
    CCLOG("setObjectCard");
    
    std::vector<Point> vecPosList;
    std::string fileName = getFileName(levelType, name);
    
    auto sprite = Sprite::create(fileName.c_str());
    if(sprite){
        Size size = sprite->getContentSize();

        if((num == 1) || (num == 5))
            vecPosList.push_back(Point(0.f, 10.f));
        
        float addMargin = 1.1f;
        
        if (num == 2) addMargin = 1.6;
        if (num == 4) addMargin = 1.6;
        if (num == 5) addMargin = 2.1f;
        
        
        size = size * addMargin;
        //Size(size.width+(size.width*addMargin),
          //          size.height+(size.height*addMargin));
        
        int height = 0;
        int addValue = 0;
        if(num > 1 && num <= 3) height = num;
        else if(num <= 6) height = num/2;
        else{
            addValue = (num%3 > 0) ? 1:0;
            height = (num/3)+addValue;
        }
        
        float startY = m_pTopLayer->getStartPoint(height, 10.f, size.height);
        for(int i = 0; i < height; ++i){
            int col = (num/height)+addValue;
            int width = MIN(num-(i*col), col);
            
            float startX = m_pTopLayer->getStartPoint(width, 0.f, size.width);
            for(int j = 0; j < width; ++j){
                vecPosList.push_back(Point(startX, startY));
                startX += size.width;
            }
            
            startY += size.height;
        }
        
        for(unsigned int i = 0; i < vecPosList.size(); ++i){
            auto pObject = Sprite::create(fileName.c_str());
            if(pObject){
                pObject->setPosition(vecPosList[i]);
                this->addChild(pObject, K_ORDER_OBJECT);
            }
        }

    }
}

std::string EquationMakerObject::getFileName(int levelType, std::string name, int num){
    std::string ret = "";
    if(levelType == 3){
        ret = "EquationMaker/counting_"+name+".png";
    }
    else if(levelType == 5){
        std::string bowl = "";
        if(num > 10) bowl = "_bowl";
        
        ret = "EquationMaker/eq_"+name+bowl+".png";
    }
    
    return ret;
}

void EquationMakerObject::setShadowVisible(bool isVisible){
    m_pShadowSprite->setVisible(isVisible);
    m_pLineSprite->setVisible(isVisible);
}

void EquationMakerObject::setSlotIn(bool isSlotIn){
    m_isSlotIn = isSlotIn;
}

int EquationMakerObject::getRowType(){
    return m_RowType;
}

Point EquationMakerObject::getOriginPos(){
    return m_OriginPos;
}



int EquationMakerObject::getType(){
    return m_Type;
}

bool EquationMakerObject::isSlotIn(){
    return m_isSlotIn;
}

bool EquationMakerObject::isTouchedIn(Point pos){
    if(m_isSlotIn)
        return false;
    
    bool ret = false;
    
    Rect spriteRect = m_pCardSprite->getBoundingBox();
    spriteRect.origin = m_OriginPos;
    spriteRect.origin.x -= (spriteRect.size.width/ 2.0f);
    spriteRect.origin.y -= (spriteRect.size.height/ 2.0f);
    ret = spriteRect.containsPoint(pos);
    
    return ret;
}

void EquationMakerObject::setOriginPos(Point pos){
    m_OriginPos = pos;
}

void EquationMakerObject::setTopLayer(EquationMakerScene* pLayer){
    m_pTopLayer = pLayer;
}

void EquationMakerObject::onExit(){
	Node::onExit();

}
