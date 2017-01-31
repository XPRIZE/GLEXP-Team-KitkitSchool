//
//  PopupBase.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 11/1/16.
//
//

#include "PopupBase.hpp"


PopupBase::~PopupBase()
{
    CC_SAFE_RELEASE_NULL(backgroundView);
    CC_SAFE_RELEASE_NULL(dismissCallback);
}

PopupBase* PopupBase::create(Node* parent)
{
    PopupBase *popup = new (std::nothrow) PopupBase();
    if (popup && popup->init(parent))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

PopupBase* PopupBase::create(Node* parent, Size size)
{
    PopupBase *popup = new (std::nothrow) PopupBase();
    if (popup && popup->init(parent, size))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool PopupBase::init(Node *parent)
{
    if (!Node::init()) {
        return false;
    }
    
    if (parent==nullptr) parent = Director::getInstance()->getRunningScene();
    
    showFromTop = false;
    allowDismissByTouchingOutside = false;
    this->parentView = parent;
    backgroundView = Node::create();// Control::create();
//    backgroundView->setPosition(Vec2::ANCHOR_MIDDLE);
    backgroundView->setContentSize(parentView->getContentSize());
//    backgroundView->setPosition(parentView->getContentSize()/2.f);
    //    CCLOG("parent Size : (%f,%f), bg Size : (%f,%f)", parentView->getContentSize().width,parentView->getContentSize().height, backgroundView->getContentSize().width, backgroundView->getContentSize().height);
    //backgroundView->addTargetWithActionForControlEvents(this, cccontrol_selector(PopupBase::backviewTouched), Control::EventType::TOUCH_DOWN);
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [] (cocos2d::Touch*, cocos2d::Event*) {
        return true;
    };
    listener->onTouchEnded = [this](cocos2d::Touch* touch, cocos2d::Event* event)
    {
        if (allowDismissByTouchingOutside) {
            dismiss(true);
        }
    };
    
    
    EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, backgroundView);
    
    bgColor = LayerColor::create(Color4B(0, 0, 0, 255*0.8f));
    bgColor->setContentSize(backgroundView->getContentSize());
    backgroundView->addChild(bgColor);
    backgroundView->retain();
    
    setContentSize(parentView->getContentSize()*2.f/3.f);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(parentView->getContentSize()/2.f);
    dismissCallback = nullptr;
    onloadCallback = nullptr;
    //    CCLOG("parent Size : (%f,%f), bg Size : (%f,%f)", parentView->getContentSize().width,parentView->getContentSize().height, backgroundView->getContentSize().width, backgroundView->getContentSize().height);
    
    _listener = nullptr;
    
    
    
    return true;
    
}

bool PopupBase::init(Node *parent, Size size)
{
    if (!init(parent)) {
        return false;
    }
    
    setContentSize(size);
    
    return true;
    
}



void PopupBase::close()
{
    dismiss(true);
}

void PopupBase::show(Node *_parentModal, bool animate)
{
    
    _isActive = true;
    float screenHeight = parentView->getContentSize().height;
    float offset = showFromTop ? -1*screenHeight : screenHeight;
    
    parentView->addChild(backgroundView);
    parentView->addChild(this);
    
    
    if (animate) {
        setPositionY(getPositionY() - offset);
        backgroundView->setOpacity(0);
        
        auto fadeIn = EaseIn::create(FadeIn::create(0.2), 2);
        auto moveBy = EaseIn::create(MoveBy::create(0.2, Vec2(0, offset)), 2);
        backgroundView->runAction(fadeIn);
        
        if(onloadCallback){
            this->runAction(Sequence::create(moveBy, onloadCallback, NULL));
        }else{
            this->runAction(moveBy);
        }
    }
}


void PopupBase::show(Node *_parentModal, bool animate, float delay)
{
    
    _isActive = true;
    
    bgColor->setOpacity(0);
    
    parentView->addChild(backgroundView);
    parentView->addChild(this);//, 100);
    
    setVisible(false);
    auto delayTime = DelayTime::create(delay);
    auto seq = Sequence::create(delayTime, CallFunc::create([this,animate](){
        float screenHeight = parentView->getContentSize().height;
        float offset = showFromTop ? -1*screenHeight : screenHeight;
        if (_isActive) {
            bgColor->setOpacity(255*0.8f);
            setVisible(true);
            if (animate) {
                setPositionY(getPositionY() - offset);
                backgroundView->setOpacity(0);
                
                auto fadeIn = EaseIn::create(FadeIn::create(0.2), 2);
                auto moveBy = EaseIn::create(MoveBy::create(0.2, Vec2(0, offset)), 2);
                backgroundView->runAction(fadeIn);
                if(onloadCallback){
                    this->runAction(Sequence::create(moveBy, onloadCallback, NULL));
                }else{
                    this->runAction(moveBy);
                }
            }
            
        }
        
    }), NULL);
    runAction(seq);
    
}

void PopupBase::show(Node *_parentModal, bool animate, cocos2d::CallFunc *callback)
{
    dismissCallback = callback;
    if (dismissCallback) {
        dismissCallback->retain();
    }
    
    show(_parentModal, animate);
}

void PopupBase::dismiss(bool animate)
{
    _isActive = false;
    
    if(animate)
    {
        float screenHeight = parentView->getContentSize().height;
        float offset = showFromTop ? -1*screenHeight : screenHeight;
        auto moveBy = EaseIn::create(MoveBy::create(0.2, Vec2(0,-offset)), 2);
        auto fadeOut = EaseIn::create(FadeOut::create(0.2), 2);
        backgroundView->runAction(fadeOut);
        auto seq = Sequence::create(moveBy, CallFunc::create([this](){
            if(dismissCallback)
            {
                dismissCallback->execute();
            }
            backgroundView->removeFromParent();
            this->removeFromParent();
            
        }), NULL);
        this->runAction(seq);
        
    }
    else {
        if(dismissCallback)
        {
            dismissCallback->execute();
        }
        backgroundView->removeFromParent();
        this->removeFromParent();
    }
}

void PopupBase::backKeyPressed()
{
    this->dismiss(true);
}

void PopupBase::setAllowDismissByTouchingOutside(bool allow)
{
    allowDismissByTouchingOutside = allow;
    
    if (allowDismissByTouchingOutside) {
        _listener = EventListenerTouchOneByOne::create();
        _listener->setSwallowTouches(true);
        _listener->onTouchBegan = [&](cocos2d::Touch* touch, cocos2d::Event* event) {
            Vec2 p = touch->getLocation();
            
            Rect rect = this->getBoundingBox();
            //CCLOG("rect= %f %f %f %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
            
            if(rect.containsPoint(p)) {
                
                return false;
            }
            
            return true;
        };
        _listener->onTouchEnded = [&] (cocos2d::Touch* touch, cocos2d::Event* event) {
            auto t = touch->getLocation();
            //CCLOG("touches... %f, %f", t.x, t.y);
            
            if (allowDismissByTouchingOutside && this->getBoundingBox().containsPoint(t) == false) {
                dismiss(true);
            }
            
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);
    }
    else {
        if (_listener) {
            getEventDispatcher()->removeEventListener(_listener);
        }
        
    }
}

