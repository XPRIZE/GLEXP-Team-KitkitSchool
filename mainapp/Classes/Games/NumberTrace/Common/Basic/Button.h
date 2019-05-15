//
//  Button.h on Aug 6, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/ui/UIButton.h>
#include <cocos/base/CCTouch.h>
#include <cocos/base/CCEvent.h>
#include <functional>


namespace todoschool {
    
class Button: public cocos2d::ui::Button {
public:
    typedef cocos2d::ui::Button Super;
    typedef std::function<void(cocos2d::Ref*,
                               cocos2d::ui::Widget::TouchEventType,
                               cocos2d::Touch* touch,
                               cocos2d::Event* event)> TouchCallbackType;
    typedef std::function<void(cocos2d::Ref*,
                               cocos2d::Touch* touch,
                               cocos2d::Event* event)> ClickCallbackType;
    
protected:
    TouchCallbackType _touchCallback;
    ClickCallbackType _clickCallback;

public:
    static Button* create();
    static Button* create(const std::string& normalImage,
                          const std::string& selectedImage = "",
                          const std::string& disableImage = "",
                          TextureResType texType = TextureResType::LOCAL);

    void addTouchEventListener(const TouchCallbackType& callback);
    void addClickEventListener(const ClickCallbackType& callback);

    // NB(xenosoz, 2016): What I really want here is just override Widget::pushDownEvent,
    //   I just can't find a way without modyfing the cocos source code itself
    //   which is not a viable solution by the time now.
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) override;
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) override;
    
protected:
    virtual void pushDownEvent(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void moveEvent(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void releaseUpEvent(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void cancelUpEvent(cocos2d::Touch* touch, cocos2d::Event* event);
};

}  // namespace todoschool
