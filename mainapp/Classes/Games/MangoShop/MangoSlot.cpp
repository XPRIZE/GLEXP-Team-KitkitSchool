//
//  MangoSlot.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#include "MangoSlot.hpp"

namespace MangoShop
{
    const string kMangoSlotImageLandscape = "MangoShop/mango-shop_image_mango_horizontal_guide.png";
    const string kMangoSlotImagePortrait = "MangoShop/mango-shop_image_mango_vertical_guide.png";
    
    MangoSlot::MangoSlot():
    targetMango(nullptr),
    isDraggable(true)
    {
        
    }
    
    bool MangoSlot::init()
    {
        if (!Node::init()) { return false; }
        return true;
    }
    
    void MangoSlot::initWithOrientation(bool isLandscape = true)
    {
        _body = Sprite::create(isLandscape ? kMangoSlotImageLandscape : kMangoSlotImagePortrait);
        this->setContentSize(_body->getContentSize());
        _body->setPosition(this->getContentSize() / 2);
        this->addChild(_body);
    }
    
} // end namespace
