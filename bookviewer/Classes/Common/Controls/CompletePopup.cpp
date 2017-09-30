//
//  CompletePopup.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 6/29/16.
//
//

#include "CompletePopup.hpp"
#include "ui/UIButton.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

bool CompletePopup::init() {
    if (!Node::init()) return false;
    
//    Size winSize = Director::getInstance()->getWinSize();
    float designHeight = 1800;
    Size deviceSize = Director::getInstance()->getWinSize();
    Size winSize = Size(deviceSize.width/deviceSize.height*designHeight, designHeight);
    setContentSize(winSize);
    setScale(deviceSize.height/designHeight);
    
    
    auto blocker = ui::Widget::create();
    blocker->setAnchorPoint(Vec2::ZERO);
    blocker->setContentSize(winSize);
    blocker->setTouchEnabled(true);
    blocker->setSwallowTouches(true);

    addChild(blocker);
    
    _backView = LayerColor::create(Color4B(0, 0, 0, 128), winSize.width, winSize.height);
    _backView->setOpacity(0);
    addChild(_backView);
    
    _mainView = Node::create();
    addChild(_mainView);
    _mainView->setContentSize(winSize);
    
    auto bg = Sprite::create("Common/Controls/CompletePopup/popup_background.png");
    bg->setPosition(winSize/2);
    _mainView->addChild(bg);

    
    Point p = winSize/2;
    
    auto l1 = Sprite::create("Common/Controls/CompletePopup/popup_beam_1.png");
    l1->setPosition(p);
    l1->runAction(RepeatForever::create(RotateBy::create(0.5, 30)));
    _mainView->addChild(l1);
    
    auto l2 = Sprite::create("Common/Controls/CompletePopup/popup_beam_2.png");
    l2->setPosition(p);
    l2->runAction(RepeatForever::create(RotateBy::create(0.5, -30)));
    _mainView->addChild(l2);
    
    
    auto star = Sprite::create("Common/Controls/CompletePopup/popup_star.png");
    star->setPosition(winSize/2);
    _mainView->addChild(star);
    
    std::string text = (IS_ENGLISH) ? "Great!" : "Vizuri!";
    
    auto label = Label::createWithTTF(text, "fonts/TodoMainCurly.ttf", 200);
    label->setTextColor(Color4B(84, 46, 26, 255));
    label->setPosition(Vec2(p.x, p.y+420));
    _mainView->addChild(label);
    
    auto btn = ui::Button::create();
    btn->loadTextures("Common/Controls/CompletePopup/popup_button_inactivated.png", "Common/Controls/CompletePopup/popup_button_activated.png");
    btn->setPosition(Vec2(p.x, p.y-450));
    btn->addClickEventListener([this](Ref*) {
        
        this->dismiss();
    });
    
    _mainView->addChild(btn);
    
    
    
    return true;
    
}

void CompletePopup::show(float delay, std::function<void(void)> callback) {
    Size winSize = getContentSize();
    
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(this);
    
    
    _mainView->setPosition(Vec2(0, winSize.height));
    auto seq = Sequence::create(DelayTime::create(delay),
                                EaseOut::create(MoveTo::create(0.2, Vec2(0, 0)), 2.0),
                                EaseIn::create(MoveBy::create(0.1, Vec2(0, -30)), 2.0),
                                EaseOut::create(MoveBy::create(0.1, Vec2(0, 30)), 2.0),
                                nullptr);
                                
    _mainView->runAction(seq);
    
    _backView->runAction(Sequence::create(
                                          DelayTime::create(delay),
                                          FadeTo::create(0.2, 128),
                                          nullptr)
                                          );
    

    
    
    
    
    
    scheduleOnce([](float) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Common/SFX_ChildrenCheerNew.m4a");
        
    }, delay+0.1, "wowSound");;
    
    
    _callback = callback;
    
}

void CompletePopup::dismiss() {
    Size winSize = getContentSize();
    auto seq = Sequence::create(
                                EaseIn::create(MoveBy::create(0.1, Vec2(0, -30)), 2.0),
                                EaseOut::create(MoveTo::create(0.2, Vec2(0, winSize.height)), 2.0),
                                CallFunc::create([this](){
        this->removeFromParent();
    }),
                                nullptr);
    _mainView->runAction(seq);
    
    _backView->runAction(Sequence::create(DelayTime::create(0.3), FadeOut::create(0.2), nullptr));
    
    if (_callback) {
        _callback();
    }
    
}
