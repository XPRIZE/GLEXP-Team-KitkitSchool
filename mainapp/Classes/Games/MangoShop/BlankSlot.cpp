//
//  BlankSlot.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 3/9/18.
//

#include "BlankSlot.hpp"

namespace MangoShop
{
    const string kSelectedEffectTextureString = "MangoShop/select-slot.png";
    const string kNotSelectedEffectTextureString = "MangoShop/empty-slot.png";
    
    BlankSlot::BlankSlot():
    isDone(false)
    {
        
    }
    
    bool BlankSlot::init()
    {
        if (!Node::init()) { return false; }
        
        _body = Sprite::create("MangoShop/empty-slot.png");
        this->setContentSize(_body->getContentSize());
        _body->setPosition(this->getContentSize() / 2);
        this->addChild(_body);
        
        return true;
    }
    
    void BlankSlot::setSelectEffect(bool selected)
    {
        _body->setTexture(selected ? kSelectedEffectTextureString : kNotSelectedEffectTextureString);
    }
    
} // end namespace
