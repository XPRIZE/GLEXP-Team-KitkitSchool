//
//  NumberCard.cpp
//  KitkitSchool-mobile
//
//  Created by YongSoo Hong on 03/29/18
//

#include "NumberCard.hpp"
#include "Utils/TodoUtil.h"

namespace MissingNumber
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    const std::string ROOT_RESOURCE_PATH = "Games/MissingNumber";
#else
    const std::string ROOT_RESOURCE_PATH = "MissingNumber";
#endif
    bool NumberCard::isActive = true;
    
    NumberCard::NumberCard():
    onTouchBegan(nullptr),
    onTouchMoved(nullptr),
    onTouchEnded(nullptr)
    {
        
    }

    bool NumberCard::init()
    {
        if (!Node::init()) { return false; }
        
        this->setCascadeOpacityEnabled(true);
        return true;
    }

    void NumberCard::initWithNumberString(string numberString, bool bSmall, bool bMove, float scale)
    {
        _numberString = TodoUtil::trim(numberString);

        auto body = Sprite::create(ROOT_RESOURCE_PATH + (bSmall == true ? "/card_forSingleDigit.png" : "/card_forThreeDigit.png"));
                                   
        body->setCascadeOpacityEnabled(true);
        body->setScale(scale);
        this->setContentSize(body->getContentSize() * scale);
        body->setPosition(this->getContentSize() / 2);
        this->addChild(body);
        
        _label = TodoUtil::createLabel(_numberString, 167.f, Size::ZERO, "fonts/OpenSans-Bold.ttf", Color4B(85, 55, 52, 255));
        _label->setCascadeOpacityEnabled(true);
        _label->setScale(scale);
        _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _label->setPosition(body->getContentSize() / 2);
        body->addChild(_label);
        
        if (bMove == true)
        {
            auto *listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](Touch* T, Event* E) {
                if (NumberCard::isActive == false)
                    return false;
                
                auto P = getParent();
                auto pos = P->convertToNodeSpace(T->getLocation());
                if (this->getBoundingBox().containsPoint(pos)) {
                    NumberCard::isActive = false;
                    if(onTouchBegan) onTouchBegan();
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
//                NumberCard::isActive = true;
                if(onTouchEnded) onTouchEnded();
            };
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        }
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
