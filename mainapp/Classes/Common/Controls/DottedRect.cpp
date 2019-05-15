//
//  DottedRect.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 1/6/17.
//
//

#include "DottedRect.hpp"

DottedRect* DottedRect::create(Size prefferedSize)
{
    DottedRect *popup = new (std::nothrow) DottedRect();
    if (popup && popup->init(prefferedSize))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool DottedRect::init(Size prefferedSize)
{
    if (!Node::init()) {
        return false;
    }
    
    
    const float cornerWidth = 11;
    const float cornerHalf = cornerWidth/2;
    const float unit = 16;
    
    
    float horiLength = ceil((prefferedSize.width - 2*cornerWidth) / unit)*unit;
    float vertLength = ceil((prefferedSize.height - 2*cornerWidth) / unit)*unit;
    Size actualSize = Size(horiLength + 2*cornerWidth, vertLength + 2*cornerWidth);
    
    float hCenter = actualSize.width/2;
    float vCenter = actualSize.height/2;
    float offset = 4;
    
    
    setContentSize(actualSize);
    

    
    _face = Node::create();
    _face->setContentSize(actualSize);
    addChild(_face);
    
    
    
    // openGL에서 texture repeat를 사용하기 위해서는 texture의 width/height가 각각 2의 승수가 되어야 하기 때문에
    // 리소스를 임의로 편집해서 맞췄습니다.
    
    Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    
    Texture2D *horiTex = TextureCache::getInstance()->addImage("Common/Controls/dotted_line_16_hori.png");
    horiTex->setTexParameters(params);
    
    Texture2D *vertTex = TextureCache::getInstance()->addImage("Common/Controls/dotted_line_16_vert.png");
    vertTex->setTexParameters(params);



    
    //auto c1 = Sprite::createWithTexture(cornerTex, Rect(0, 0, cornerWidth, cornerWidth));
    //auto c1 = Sprite::create("Common/Controls/dotted_line_16_corner.png", Rect(0, 0, cornerWidth, cornerWidth));
    auto c1 = Sprite::create("Common/Controls/dotted_line_16_corner.png",  Rect(0, cornerWidth, cornerWidth, cornerWidth));
    c1->setPosition(Vec2(cornerHalf, cornerHalf));
    _face->addChild(c1);
    
    //auto c2 = Sprite::createWithTexture(cornerTex, Rect(cornerWidth, 0, cornerWidth, cornerWidth));
    auto c2 = Sprite::create("Common/Controls/dotted_line_16_corner.png", Rect(cornerWidth, cornerWidth, cornerWidth, cornerWidth));
    c2->setPosition(Vec2(actualSize.width-cornerHalf, cornerHalf));
    _face->addChild(c2);
    
    //auto c3 = Sprite::createWithTexture(cornerTex, Rect(0, cornerWidth, cornerWidth, cornerWidth));
    auto c3 = Sprite::create("Common/Controls/dotted_line_16_corner.png", Rect(0, 0, cornerWidth, cornerWidth));
    
    c3->setPosition(Vec2(cornerHalf, actualSize.height-cornerHalf));
    _face->addChild(c3);
    
    //auto c4 = Sprite::createWithTexture(cornerTex, Rect(cornerWidth, cornerWidth, cornerWidth, cornerWidth));
    auto c4 = Sprite::create("Common/Controls/dotted_line_16_corner.png", Rect(cornerWidth, 0, cornerWidth, cornerWidth));
    
    c4->setPosition(Vec2(actualSize.width-cornerHalf, actualSize.height-cornerHalf));
    _face->addChild(c4);

   
    
    auto h1 = Sprite::createWithTexture(horiTex, Rect(0, 0, horiLength, 8));
    h1->setPosition(Vec2(hCenter, offset));
    _face->addChild(h1);
    
    auto h2 = Sprite::createWithTexture(horiTex, Rect(0, 0, horiLength, 8));
    h2->setPosition(Vec2(hCenter, actualSize.height-offset));
    _face->addChild(h2);
    
    auto v1 = Sprite::createWithTexture(vertTex, Rect(0, 0, 8, vertLength));
    v1->setPosition(Vec2(offset, vCenter));
    _face->addChild(v1);
    
    auto v2 = Sprite::createWithTexture(vertTex, Rect(0, 0, 8, vertLength));
    v2->setPosition(Vec2(actualSize.width-offset, vCenter));
    _face->addChild(v2);
    
    
    

    

   
    
    return true;
}


