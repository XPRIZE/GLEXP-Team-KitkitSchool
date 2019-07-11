//
//  WordBlock.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/18/16.
//
//

#include "WordBlock.hpp"
#include "Utils/TodoUtil.h"
#include "FillTheBlanksScene.hpp"
#include "Common/Controls/DottedRect.hpp"

namespace WordBlockNamespace
{
    const string fontName = FONT_MUKTA_BOLD;
    const float fontSize = 60.f;
}

using namespace WordBlockNamespace;

Size WordBlock::wordBlockSize = Size(446, 128); //Size(446, 154);

void WordBlock::setWordBlockWidth(float width)
{
    // 반복되는 패턴이 안깨지게 하기 위해 16단위로 올려줍니다
    wordBlockSize.width = ceil(width/16)*16;
}



bool WordBlock::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    setContentSize(wordBlockSize);
    
    // 구현의 편의를 위해서 페이지에서 각 단어의 좌표 기준을 topleft로 잡았기 때문에 요놈도 맞춰줍니다. 
    setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    
    _snapped = false;
    _pair = nullptr;
    
    return true;
}


void WordBlock::initBlank(string word)
{
    _word = word;
    
    auto face = Node::create();
    face->setContentSize(wordBlockSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;
    
    
    addChild(_face);
    
    _face->addChild(DottedRect::create(wordBlockSize));

    
    return;
    
    
    
    
    /*
    
    // openGL에서 texture repeat를 사용하기 위해서는 texture의 width/height가 각각 2의 승수가 되어야 하기 때문에
    // 리소스를 임의로 편집해서 맞췄습니다.
    Texture2D *texture = TextureCache::getInstance()->addImage("ComprehensionTest/FillTheBlanks/emptybox_mid.png");
    Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE };
    texture->setTexParameters(params);
    

    
    auto tl = Sprite::create("ComprehensionTest/FillTheBlanks/emptybox_left.png");
    tl->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    auto tlw = tl->getContentSize().width;
    _face->addChild(tl);
    
    
    auto tr = Sprite::create("ComprehensionTest/FillTheBlanks/emptybox_right.png");
    tr->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    auto trw = tr->getContentSize().width;
    auto trp = Vec2(wordBlockSize.width-trw, 0);
    tr->setPosition(trp);
    _face->addChild(tr);
    
    auto tt = Sprite::createWithTexture(texture, Rect(0, 0, wordBlockSize.width-tlw-trw, wordBlockSize.height));
    tt->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    auto ttp = Vec2(tlw, 0);
    tt->setPosition(ttp);
    _face->addChild(tt);
    */
    
 

}

void WordBlock::initWord(string word)
{
    _word = word;
    
    auto shadow = Scale9Sprite::create("ComprehensionTest/FillTheBlanks/comprehention_filltheblank_wordcard_shadow.png");
    shadow->setPreferredSize(wordBlockSize);
    shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shadow = shadow;
    addChild(_shadow);
    
    auto face = Scale9Sprite::create("ComprehensionTest/FillTheBlanks/comprehention_filltheblank_wordcard.png");
    face->setPreferredSize(wordBlockSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;
    addChild(_face);
    
    auto l = TodoUtil::createLabel(word, fontSize, Size::ZERO, fontName, Color4B::BLACK);
    l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    l->setPosition(wordBlockSize/2);
    _face->addChild(l);
    
    
    // 터치 리스너는 각 객체에 붙여주는것이 좋습니다.
    // addEventListenerWithSceneGraphPriority로 붙이면 화면에 표시되는 순서대로 (위부터) 방문하기 때문에
    // 밑에 깔려 있는 객체가 선택되는 것을 쉽게 방지할 수 있습니다.
    
    auto *listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) {
        
        
        // Touch의 Location은 world좌표계이고, getBoundingBox는 부모 좌표계이기 때문에 둘을 맞춰줄 필요가 있습니다.
        auto P = getParent();
        auto pos = P->convertToNodeSpace(T->getLocation());
        if (this->getBoundingBox().containsPoint(pos)) {
        
            this->setPicked(true);
            
            // 원위치로 되돌릴 일이 있을 때를 위하여 원래 위치를 저장해둡니다.
            this->_originPos = getPosition();
            
            
            // 선택된 객체를 위로 올려줍니다.
            
            this->getParent()->reorderChild(this, this->getLocalZOrder());
//            this->retain();
//            this->removeFromParent();
//            P->addChild(this);
//            this->release();
            
            if (onCheckTargetBegan) {
                onCheckTargetBegan();
            }
            
            return true;
        }
        return false;
    };
    
    listener->onTouchMoved = [this](Touch* T, Event* E) {

        // getDelta()로는 world좌표계상에서 touch의 차이를 받아오게 되기 때문에, 부모좌표계상에서의 차이를 알기 위해
        // previousLocation과 Location을 받아서 각각 변환을 해준 다음에 차이를 구합니다. 
        auto P = getParent();
        auto pl = P->convertToNodeSpace(T->getPreviousLocation());
        auto cl = P->convertToNodeSpace(T->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        
    };
    
    listener->onTouchEnded = [this](Touch* T, Event* E) {
        // 놓았을 때 빈칸에 들어갔는지 확인하기 위한 callback입니다.
        setPicked(false);
        
        if (onCheckTargetEnd) {
            onCheckTargetEnd();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


void WordBlock::setPicked(bool picked)
{

    if (picked) {
        // snap된 놈을 들었을 때 다시 그림자가 나타나게 합니다.
        _shadow->setVisible(true);
        
        _face->setPosition(0, 5);
        _shadow->setPosition(5, -5);
        
    } else {
        // snap된 놈을 놓았을 떄 그림자를 다시 숨깁니다.
        if (_snapped) _shadow->setVisible(false);
        _face->setPosition(Vec2::ZERO);
        _shadow->setPosition(Vec2::ZERO);
    }
}

void WordBlock::setSnapped(bool snapped)
{
    _snapped = snapped;
    _shadow->setVisible(!snapped);
    
}

void WordBlock::returnToOrigin()
{
    setPosition(_originPos);
    
}
