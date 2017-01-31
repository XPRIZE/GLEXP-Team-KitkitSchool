//
//  Button.h on Aug 6, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Button.h"
#include <cocos/2d/CCCamera.h>


using namespace cocos2d;
using namespace cocos2d::ui;

namespace todoschool {

Button* Button::create() {
    // NB(xenosoz, 2016): Code in this function from `cocos2d/cocos/ui/UIButton.cpp'.
    // Copyright (c) 2013-2014 Chukong Technologies Inc.
    Button* widget = new (std::nothrow) Button();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

Button* Button::create(const std::string &normalImage,
                       const std::string& selectedImage ,
                       const std::string& disableImage,
                       TextureResType texType)
{
    // NB(xenosoz, 2016): Code in this function from `cocos2d/cocos/ui/UIButton.cpp'.
    // Copyright (c) 2013-2014 Chukong Technologies Inc.
    Button *btn = new (std::nothrow) Button;
    if (btn && btn->init(normalImage,selectedImage,disableImage,texType))
    {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return nullptr;
}

void Button::addTouchEventListener(const TouchCallbackType& callback) {
    _touchCallback = callback;
}

void Button::addClickEventListener(const ClickCallbackType& callback) {
    _clickCallback = callback;
}

bool Button::onTouchBegan(Touch* touch, Event* event) {
    // NB(xenosoz, 2016): Code in this function from cocos2d/cocos/ui/UIWidget.cpp.
    // Copyright (c) 2013-2014 Chukong Technologies Inc.

    _hitted = false;
    if (isVisible() && isEnabled() && isAncestorsEnabled() && isAncestorsVisible(this)) {
        _touchBeganPosition = touch->getLocation();
        auto camera = Camera::getVisitingCamera();
        if(hitTest(_touchBeganPosition, camera, nullptr))
        {
            if (isClippingParentContainsPoint(_touchBeganPosition)) {
                _hittedByCamera = camera;
                _hitted = true;
            }
        }
    }

    if (!_hitted)
        return false;

    setHighlighted(true);

    // Propagate touch events to its parents
    if (_propagateTouchEvents)
        this->propagateTouchEvent(TouchEventType::BEGAN, this, touch);
    
    pushDownEvent(touch, event);
    return true;
}

void Button::onTouchMoved(Touch* touch, Event* event) {
    // NB(xenosoz, 2016): Code in this function from cocos2d/cocos/ui/UIWidget.cpp.
    // Copyright (c) 2013-2014 Chukong Technologies Inc.

    _touchMovePosition = touch->getLocation();
    
    setHighlighted(hitTest(_touchMovePosition, _hittedByCamera, nullptr));
    
    // Propagate touch events to its parents
    if (_propagateTouchEvents)
        this->propagateTouchEvent(TouchEventType::MOVED, this, touch);
    
    moveEvent(touch, event);
}

void Button::onTouchEnded(Touch* touch, Event* event) {
    // NB(xenosoz, 2016): Code in this function from cocos2d/cocos/ui/UIWidget.cpp.
    // Copyright (c) 2013-2014 Chukong Technologies Inc.

    _touchEndPosition = touch->getLocation();
    
    /*
     * Propagate touch events to its parents
     */
    if (_propagateTouchEvents)
    {
        this->propagateTouchEvent(TouchEventType::ENDED, this, touch);
    }
    
    bool highlight = _highlight;
    setHighlighted(false);
    
    if (highlight)
        releaseUpEvent(touch, event);
    else
        cancelUpEvent(touch, event);
}

void Button::onTouchCancelled(Touch* touch, Event* event) {
    setHighlighted(false);
    cancelUpEvent(touch, event);
}

void Button::pushDownEvent(Touch* touch, Event* event) {
    Super::pushDownEvent();

    this->retain();
    if (_touchCallback)
        _touchCallback(this, TouchEventType::BEGAN, touch, event);
    this->release();
}

void Button::moveEvent(Touch* touch, Event* event) {
    Super::moveEvent();
    
    this->retain();
    if (_touchCallback)
        _touchCallback(this, TouchEventType::MOVED, touch, event);
    this->release();
}

void Button::releaseUpEvent(Touch* touch, Event* event) {
    Super::releaseUpEvent();

    this->retain();

    if (_touchCallback)
        _touchCallback(this, TouchEventType::ENDED, touch, event);
    
    if (_clickCallback)
        _clickCallback(this, touch, event);

    this->release();
}

void Button::cancelUpEvent(Touch* touch, Event* event) {
    Super::cancelUpEvent();

    this->retain();
    
    if (_touchCallback)
        _touchCallback(this, TouchEventType::CANCELED, touch, event);

    this->release();
}


}  // namespace todoschool