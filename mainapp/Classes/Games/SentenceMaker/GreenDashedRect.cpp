//
//  GreenDashedRect.cpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 13/02/2018.
//

#include "GreenDashedRect.hpp"
#include "WordItem.hpp"

GreenDashedRect* GreenDashedRect::create() {
    GreenDashedRect *popup = new (std::nothrow) GreenDashedRect();
    if (popup && popup->init())
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

float GreenDashedRect::getBodyWidth(float width) {
    return width-34*2;
}

bool GreenDashedRect::init() {
    if (!Node::init()) {
        return false;
    }
    _defaultWidth = 160;
    _stretched = false;
    _pair = nullptr;
    _word = "";
    
    auto bodyWidth = getBodyWidth(_defaultWidth);
    _currentBodyWidth = bodyWidth;
    setContentSize(Size(_defaultWidth, 128));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _left = Sprite::create("SentenceMaker/Sentencemaker_image_block-guideline-left.png");
    _left->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _left->setPosition(Vec2(bodyWidth/2*-1+getContentSize().width/2, getContentSize().height/2));
    addChild(_left);
    
    _right = Sprite::create("SentenceMaker/Sentencemaker_image_block-guideline-right.png");
    _right->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _right->setPosition(Vec2(bodyWidth/2+getContentSize().width/2,getContentSize().height/2));
    addChild(_right);
    
    Texture2D::TexParams params = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
    _horiTex = Director::getInstance()->getTextureCache()->addImage("SentenceMaker/Sentencemaker_image_block-guideline-center.png");
    _horiTex->setTexParameters(params);
    
    _body = Sprite::createWithTexture(_horiTex, Rect(0, 0, bodyWidth, 128));
    _body->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _body->setPosition(getContentSize()/2);
    addChild(_body);
    return true;
    
}

void GreenDashedRect::initUI() {
    
}

void GreenDashedRect::setBodyWidth(float width) {
    this->setContentSize(Size(width, this->getContentSize().height));
    auto bodyWidth = getBodyWidth(width);
    _left->setPosition(Vec2(bodyWidth/2*-1+getContentSize().width/2, getContentSize().height/2));
    _right->setPosition(Vec2(bodyWidth/2+getContentSize().width/2,getContentSize().height/2));
    _body->setTextureRect(Rect(0, 0, bodyWidth, 128));
    _body->setPosition(getContentSize()/2);
    _stretched = (width == _defaultWidth) ? false : true;
}
