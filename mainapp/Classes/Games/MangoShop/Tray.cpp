//
//  Tray.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/7/18.
//

#include "Tray.hpp"

namespace MangoShop
{
    Tray::Tray()
    {
        
    }
    
    bool Tray::init()
    {
        if (!Node::init()) { return false; }
        
        auto body = Sprite::create("MangoShop/mango-shop_image_plate.png");
        this->setContentSize(body->getContentSize());
        body->setPosition(this->getContentSize() / 2);
        this->addChild(body);
        
        return true;
    }
    
} // end namespace
