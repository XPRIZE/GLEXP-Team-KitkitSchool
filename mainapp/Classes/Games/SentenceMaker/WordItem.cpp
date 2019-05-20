//
//  WordItem.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/18/16.
//
//

#include "WordItem.hpp"
#include "Utils/TodoUtil.h"
#include "GreenDottedRect.hpp"
#include "GreenDashedRect.hpp"

namespace WordItemNamespace
{
    const string kFontName = "fonts/mukta-bold.ttf";
    const float kFontSize = 80.f;
    const float kItemPadding = 50.f;
}

using namespace WordItemNamespace;

Size WordItem::wordItemSize = Size(446, 128); //Size(446, 154);

void WordItem::setWordItemWidth(float width)
{
    // 반복되는 패턴이 안깨지게 하기 위해 16단위로 올려줍니다
    wordItemSize.width = ceil(width/16)*16;
}

bool WordItem::_touchEnabled = true;

bool WordItem::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    setContentSize(wordItemSize);
    
    // 구현의 편의를 위해서 페이지에서 각 단어의 좌표 기준을 topleft로 잡았기 때문에 요놈도 맞춰줍니다. 
    setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    
    _snapped = false;
    _pair = nullptr;
    
    onCheckTargetBegan = nullptr;
    onCheckTargetMoved = nullptr;
    onCheckTargetEnded = nullptr;
    
    return true;
}


void WordItem::initBlank(string word)
{
    _word = word;
    
    auto tempLabel = TodoUtil::createLabel(word, kFontSize, Size::ZERO, kFontName, Color4B::BLACK);
    auto tempTotalWidth = kItemPadding * 2 + tempLabel->getContentSize().width;
    
    wordItemSize = Size(ceil(tempTotalWidth/16)*16, 128);
    setContentSize(wordItemSize);
    
    auto face = Node::create();
    face->setContentSize(wordItemSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;
    
    
    _face->addChild(GreenDottedRect::create(wordItemSize));
    
    addChild(_face);

}

void WordItem::initWord(string word)
{
    _word = word;
    
    auto tempLabel = TodoUtil::createLabel(word, kFontSize, Size::ZERO, kFontName, Color4B::BLACK);
    auto tempTotalWidth = kItemPadding * 2 + tempLabel->getContentSize().width;
    wordItemSize = Size(ceil(tempTotalWidth/16)*16, 128);
    setContentSize(wordItemSize);
    
    auto shadow = Scale9Sprite::create("SentenceMaker/sentencemaker_answerchoice_shadow.png");
    shadow->setPreferredSize(wordItemSize);
    shadow->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _shadow = shadow;
    addChild(_shadow);
    
    auto face = Scale9Sprite::create("SentenceMaker/sentencemaker_answerchoice.png");
    face->setPreferredSize(wordItemSize);
    face->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _face = face;
    addChild(_face);
    
    auto l = TodoUtil::createLabel(word, kFontSize, Size::ZERO, kFontName, Color4B::WHITE);
    l->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    l->setPosition(wordItemSize.width/2, wordItemSize.height/2+10);
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
            if (!_touchEnabled) return false;
            _touchEnabled = false;

            this->setPicked(true);
            
            // 선택된 객체를 위로 올려줍니다.
            this->getParent()->reorderChild(this, this->getLocalZOrder());
            
//
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
    
    listener->onTouchMoved = [this](Touch* touch, Event* event) {

        // getDelta()로는 world좌표계상에서 touch의 차이를 받아오게 되기 때문에, 부모좌표계상에서의 차이를 알기 위해
        // previousLocation과 Location을 받아서 각각 변환을 해준 다음에 차이를 구합니다. 
        auto P = getParent();
        auto pl = P->convertToNodeSpace(touch->getPreviousLocation());
        auto cl = P->convertToNodeSpace(touch->getLocation());
        auto delta = cl-pl;
        this->setPosition(this->getPosition()+delta);
        if (onCheckTargetMoved) {
            onCheckTargetMoved(touch);
        }
        
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        // 놓았을 때 빈칸에 들어갔는지 확인하기 위한 callback입니다.
        setPicked(false);
        _touchEnabled = true;
        
        if (onCheckTargetEnded) {
            onCheckTargetEnded(touch);
        }
    };
    
    listener->onTouchCancelled = [this](Touch*, Event*) {
        setPicked(false);
    };
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void WordItem::setOriginPos() {
    this->_originPos = getPosition();
}

void WordItem::setPicked(bool picked)
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

void WordItem::setSnapped(bool snapped)
{
    _snapped = snapped;
    _shadow->setVisible(!snapped);
    
}

void WordItem::returnToOrigin()
{
    setPosition(_originPos);
    
}
