//
//  TextBlock.cpp
//  enumaXprize
//
//  Created by HyeonGyu Yu on 20/12/2016.
//
//

#include "TextBlock.hpp"
#include "Utils/TodoUtil.h"
#include "Common/Controls/DottedRect.hpp"

TextBlock::TextBlock():
_bSlot(false)
{ }

Size TextBlock::wordBlockSize = Size(1500, 154);

bool TextBlock::init()
{
    if (!Node::init())
        return false;
    
    setContentSize(wordBlockSize);
    _snapped = false;
    _pair = nullptr;
    
    return true;
}

void TextBlock::initAsSlot(std::string text)
{
    CCLOG("initAsSlot");
    _bSlot = true;
    _solutionText = text;
    
    auto face = Node::create();
    face->setContentSize(wordBlockSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;
    
    addChild(_face);
    
    _face->addChild(DottedRect::create(wordBlockSize));
}

void TextBlock::initAsBlock(std::string text)
{
    _bSlot = false;
    _text = text;
    
    auto shadow = Scale9Sprite::create("ComprehensionTest/FillTheBlanks/comprehention_filltheblank_wordcard_shadow.png");
    shadow->setPreferredSize(wordBlockSize);
    shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shadow = shadow;
    addChild(_shadow);
    
//    _shadow = Sprite::create("ComprehensionTest/Reordering/comprehensive_shadow.png");
//    _shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
//    addChild(_shadow);
    
    auto face = Scale9Sprite::create("ComprehensionTest/FillTheBlanks/comprehention_filltheblank_wordcard.png");
    face->setPreferredSize(wordBlockSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;
    _face->setContentSize(face->getContentSize());
    addChild(_face);
    
    float labelCorrectionX = 80.f;
    auto label = TodoUtil::createLabel(text, 50, Size::ZERO, "fonts/TodoSchoolV2.ttf", Color4B(56, 56, 56, 255));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    
    float scaleFactor = MIN((wordBlockSize.width - labelCorrectionX * 2) / label->getContentSize().width, 1.0f);
    label->setScale(scaleFactor);
    label->setPosition(labelCorrectionX, _face->getContentSize().height / 2);
    _face->addChild(label);
    
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
            this->retain();
            this->removeFromParent();
            P->addChild(this);
            this->release();
            
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
        
        if (onCheckTarget) {
            onCheckTarget();
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void TextBlock::setPicked(bool picked)
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

void TextBlock::setSnapped(bool snapped)
{
    _snapped = snapped;
    _shadow->setVisible(!snapped);
    
}

void TextBlock::returnToOrigin()
{
    setPosition(_originPos);
}

std::string TextBlock::getValue()
{
    if (_bSlot) return "";
    return _text;
}

std::string TextBlock::getSolutionValue()
{
    if (!_bSlot) return "";
    return _solutionText;
}
