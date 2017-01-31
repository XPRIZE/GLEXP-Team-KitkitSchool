//
//  CountingObject.hpp -- Various counting object for Counting game
//  TodoSchool on Nov 4, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/cocos2d.h>
#include <cocos/ui/UIWidget.h>
#include <Common/Basic/CreateFunc.h>


class CountingObject : public cocos2d::Node {
    typedef cocos2d::Node Super;

public:
    CREATE_FUNC(CountingObject);
    CountingObject();
    bool init() override;
    bool init(const std::string& imageName, int weight);

    int weight() const;
    int value() const;

    void setValue(int value);
    
public:
    std::function<void(cocos2d::ui::Widget::TouchEventType event)> onTouchEvent;
    
private:
    int _weight;
    int _value;
};
