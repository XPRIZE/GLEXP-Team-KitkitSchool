//
//  BigLabel.cpp on Jul 20, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "BigLabel.h"

using namespace std;

BigLabel*
BigLabel::createWithTTF(const std::string& Text,
                        const std::string& FontFilePath, float FontSize,
                        const Size& Dimensions /* = Size::ZERO */,
                        TextHAlignment HAlignment /* = TextHAlignment::LEFT */,
                        TextVAlignment VAlignment /* = TextVAlignment::TOP */)
{
    BigLabel* BL = BigLabel::create();
    if (!BL) { return nullptr; }

    float SR = ([&] {
        // NB(xenosoz, 2016): Lower the threshold whenever the render problem occurs.
        //   500.f: doesn't work.
        //   250.f: seems fine.
        const float Threshold = 250.f;
        if (FontSize < Threshold) { return 1.f; }
        
        return Threshold / FontSize;
    }());
    
    BL->SampleRate = SR;
    BL->ChildLabel = ([&] {
        Label* It = Label::createWithTTF(Text, FontFilePath, FontSize * SR,
                                         Dimensions * SR, HAlignment, VAlignment);
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point::ZERO);
        It->setScale(1.f / SR);

        BL->setContentSize(It->getContentSize() / SR);
        BL->addChild(It);
        return It;
    }());
   
    return BL;
}
    
bool BigLabel::init() {
    if (!Node::init()) { return false; }
    
    setCascadeOpacityEnabled(true);

    SampleRate = 1.f;
    ChildLabel = nullptr;
    
    return true;
}

void BigLabel::setContentSize(const cocos2d::Size& ContentSize) {
    Node::setContentSize(ContentSize);
    
    if (ChildLabel)
        ChildLabel->setContentSize(ContentSize * SampleRate);
}
    
void BigLabel::setColor(const cocos2d::Color3B& C) {
    Node::setColor(C);
    
    if (ChildLabel)
        ChildLabel->setColor(C);
}


