//
//  Mango.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#include "Mango.hpp"
#include "NumberCard.hpp"
namespace MangoShop
{
    bool Mango::_isOtherMovingCard = false;
    
    const string kMangoBaseImageLandscape = "MangoShop/mango-shop_image_mango_horizontal_%02d.png";
    const string kMangoBaseImagePortrait = "MangoShop/mango-shop_image_mango_vertical_%02d.png";
    
    Mango::Mango():
    isActive(true),
    targetSlot(nullptr)
    {
        
    }
    
    bool Mango::init()
    {
        if (!Node::init()) { return false; }
        
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [this](Touch* T, Event* E) {
            if (Mango::_isOtherMovingCard == true)
                return false;
            
            if (NumberCard::_isOtherMovingCard == true)
                return false;
            
            auto P = getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (this->getBoundingBox().containsPoint(pos)) {
                if (isActive == false)
                    return false;
                
                if(onTouchBegan) onTouchBegan();
                Mango::_isOtherMovingCard = true;
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
            if(onTouchMoved) onTouchMoved(delta);
        };
        
        listener->onTouchEnded = [this](Touch* T, Event* E) {
            //Mango::_isOtherMovingCard = false;
            if(onTouchEnded) onTouchEnded();
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        
        return true;
    }
    
    void Mango::initWithOrientation(bool isLandscape = true)
    {
        auto s = isLandscape ? kMangoBaseImageLandscape : kMangoBaseImagePortrait;
        int r = random(1, 10);
        _body = Sprite::create(StringUtils::format(s.c_str(), r));
        this->setContentSize(_body->getContentSize());
        _body->setPosition(this->getContentSize() / 2);
        this->addChild(_body);
    }
    
} // end namespace
