//
//  CompletePopup.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 6/29/16.
//
//

#include "CompletePopup.hpp"
#include "ui/UIButton.h"
#include "Managers/LanguageManager.hpp"
#include "Common/Basic/SoundEffect.h"


namespace CompletePopupNS {
    std::string labelText() {
        if (LanguageManager::getInstance()->isSwahili())
            return "Vizuri!";
        return "Great!";
    }
}  // namespace CompetePopupNS
using namespace CompletePopupNS;


bool CompletePopup::init() {
    if (!Node::init()) return false;
    
    //    Size winSize = Director::getInstance()->getWinSize();
    float designHeight = 1800;
    Size deviceSize = Director::getInstance()->getWinSize();
    Size winSize = Size(deviceSize.width/deviceSize.height*designHeight, designHeight);
    setContentSize(winSize);
    setScale(deviceSize.height/designHeight);
    
    
    const std::string folder = "Common/Controls/CompletePopup/";
    
    
    auto blocker = ui::Widget::create();
    blocker->setAnchorPoint(Vec2::ZERO);
    blocker->setContentSize(winSize);
    blocker->setTouchEnabled(true);
    blocker->setSwallowTouches(true);
    blocker->addClickEventListener([this](Ref*){

    });
    

    addChild(blocker);
    
    _backView = LayerColor::create(Color4B(0, 0, 0, 128), winSize.width, winSize.height);
    _backView->setOpacity(0);
    addChild(_backView);
    
    _mainView = Node::create();
    addChild(_mainView);
    _mainView->setContentSize(winSize);
    
    

    
    
    
    
    auto bg = Sprite::create(folder+"game_effect_glow.png");
    bg->setPosition(winSize/2);
    _mainView->addChild(bg);

    
    Point p = winSize/2;
    
    auto l1 = Sprite::create(folder+"game_effect_rotatingright.png");
    l1->setPosition(p);
    l1->runAction(RepeatForever::create(RotateBy::create(0.5, 30)));
    _mainView->addChild(l1);
    
    auto l2 = Sprite::create(folder+"game_effect_rotatingleft.png");
    l2->setPosition(p);
    l2->runAction(RepeatForever::create(RotateBy::create(0.5, -30)));
    _mainView->addChild(l2);
    
    
    auto star = Sprite::create(folder+"game_effect_starmedal.png");
    star->setPosition(p);
    _mainView->addChild(star);
    
    {
        auto *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [star, this](Touch* T, Event* E) {
            auto P = star->getParent();
            auto pos = P->convertToNodeSpace(T->getLocation());
            if (star->getBoundingBox().containsPoint(pos)) {
                SoundEffect::buttonEffect().play();
                this->dismiss();
                return true;
            }
            return false;
        };
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, star);
        
    }

    
    const auto blinkTime = 0.3;
    
    auto e1 = Sprite::create(folder+"game_effect_sparkle_1.png");
    e1->setPosition(p+Vec2(-230, 130));
    e1->setScale(0);
    e1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
    _mainView->addChild(e1);
    
    
    auto e2 = Sprite::create(folder+"game_effect_sparkle_1.png");
    e2->setPosition(p+Vec2(-150, 190));
    e2->setScale(0);
    e2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)),ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
    _mainView->addChild(e2);
    
    auto e3 = Sprite::create(folder+"game_effect_sparkle_1.png");
    e3->setPosition(p+Vec2(130, -30));
    e3->setScale(0);
    e3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)),ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
    _mainView->addChild(e3);
    

    
    _titleTextLabel = Label::createWithTTF(labelText(), "fonts/TodoMainCurly.ttf",
                                           100, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    _titleTextLabel->setTextColor(Color4B(255, 252, 236, 255));
    _titleTextLabel->setPosition(Vec2(p.x, p.y-250));
    _mainView->addChild(_titleTextLabel);
    
    auto btn = ui::Button::create();
    btn->loadTextures(folder+"game_effect_done_normal.png", folder+"game_effect_done_touch.png");
    btn->setPosition(Vec2(p.x, p.y-450));
    btn->addClickEventListener([this](Ref*) {
        SoundEffect::buttonEffect().play();
        this->dismiss();
    });
    
    _mainView->addChild(btn);
    
    SoundEffect::wowEffect().preload();

    
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
        SoundEffect::wowEffect().play();
     
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

void CompletePopup::setTitleText(const std::string& titleText) {
    _titleTextLabel->setString(titleText);
}

