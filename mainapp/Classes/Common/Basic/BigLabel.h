//
//  BigLabel.h on Jul 20, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#ifndef TODOSCHOOL_BASIC_BIGLABEL_H
#define TODOSCHOOL_BASIC_BIGLABEL_H

#include "cocos2d.h"

USING_NS_CC;




//
// NB(xenosoz, 2016): It's easy to break a label with large font size in cocos2d-x 3.10.
//   BigLabel tries to deal with this problem by rescaled label (w/ quality sacrifice)
//   without modifying FontAtlas::CacheTextureWidth in cocos2d/cocos/2d/CCFontAtlas.cpp
//

class BigLabel : public cocos2d::Node {
    float SampleRate;
    
    // NB(xenosoz, 2016): nullable.
    cocos2d::Label* ChildLabel;

public:
    CREATE_FUNC(BigLabel);
    
    static BigLabel*
    createWithTTF(const std::string& Text,
                  const std::string& FontFilePath, float FontSize,
                  const cocos2d::Size& Dimensions = cocos2d::Size::ZERO,
                  cocos2d::TextHAlignment HAlignment = cocos2d::TextHAlignment::LEFT,
                  cocos2d::TextVAlignment VAlignment = cocos2d::TextVAlignment::TOP);
    
    bool init() override;

    void setContentSize(const cocos2d::Size& ContentSize) override;
    void setColor(const cocos2d::Color3B& C) override;
};


#endif  // !defined(TODOSCHOOL_BASIC_BIGLABEL_H)
