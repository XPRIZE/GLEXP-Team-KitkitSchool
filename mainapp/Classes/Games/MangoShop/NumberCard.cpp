//
//  NumberCard.cpp
//  KitkitSchool-mobile
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#include "NumberCard.hpp"
#include "Mango.hpp"
#include "Utils/TodoUtil.h"

namespace MangoShop
{
    bool NumberCard::_isOtherMovingCard = false;
    
    NumberCard::NumberCard():
    onTouchBegan(nullptr),
    onTouchMoved(nullptr),
    onTouchEnded(nullptr),
    isActive(true)
    {
        
    }

    bool NumberCard::init()
    {
        if (!Node::init()) { return false; }
        
        this->setCascadeOpacityEnabled(true);
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* T, Event* E) {
            if (isActive == false)
                return false;
            
            if (NumberCard::_isOtherMovingCard == true)
                return false;
            
            if (Mango::_isOtherMovingCard == true)
                return false;

            auto P = getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (this->getBoundingBox().containsPoint(pos)) {
                if(onTouchBegan) onTouchBegan();
                NumberCard::_isOtherMovingCard = true;
                return true;
            }
            return false;
        };
        
        listener->onTouchMoved = [this](Touch* T, Event* E) {
            auto P = getParent();
            auto pl = P->convertToNodeSpace(T->getPreviousLocation());
            auto cl = P->convertToNodeSpace(T->getLocation());
            auto delta = cl-pl;
            this->setPosition(this->getPosition()+delta);
            if(onTouchMoved) onTouchMoved();
        };
        
        listener->onTouchEnded = [this](Touch* T, Event* E) {
            //NumberCard::_isOtherMovingCard = false;
            if(onTouchEnded) onTouchEnded();
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    }

    void NumberCard::initWithNumberString(string numberString, float scale = 1.f)
    {
        _numberString = numberString;
        auto body = Sprite::create("MangoShop/card_empty.png");
        body->setCascadeOpacityEnabled(true);
        body->setScale(scale);
        this->setContentSize(body->getContentSize() * scale);
        body->setPosition(this->getContentSize() / 2);
        this->addChild(body);
        
        _label = TodoUtil::createLabel(numberString, 180.f, Size::ZERO, "fonts/OpenSans-Bold.ttf", Color4B(81, 53, 24, 255 * 0.9f));
        _label->setCascadeOpacityEnabled(true);
        _label->setScale(scale);
        _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _label->setPosition(body->getContentSize() / 2);
        body->addChild(_label);
    }

    void NumberCard::setNumberString(string numberString)
    {
        _label->setString(numberString);
    }
    
    string NumberCard::getNumberString()
    {
        return _numberString;
    }
    
} // end namespace
