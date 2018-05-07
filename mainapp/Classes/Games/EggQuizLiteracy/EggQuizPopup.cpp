//
//  EggQuizPopup.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 1/10/17.
//
//

#include "EggQuizPopup.hpp"
#include "ui/CocosGUI.h"


#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"

#include "Utils/TodoUtil.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"


namespace EggQuizPopupSpace {
    const string curlyFont = "fonts/TodoMainCurly.ttf";
}

using namespace EggQuizPopupSpace;
using namespace cocos2d::ui;

EggQuizPopup* EggQuizPopup::create(Node* parent)
{
    EggQuizPopup *popup = new (std::nothrow) EggQuizPopup();
    if (popup && popup->init(parent))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

EggQuizPopup* EggQuizPopup::create(Node* parent, Size size)
{
    EggQuizPopup *popup = new (std::nothrow) EggQuizPopup();
    if (popup && popup->init(parent, size))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}

void EggQuizPopup::setCommon(char category, int level)
{
    auto winSize = getContentSize();
    

    
    auto bg = Sprite::create("EggQuizLiteracy/Popup/popup_window_bg.png");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    bg->setPosition(Vec2(winSize.width/2, winSize.height));
    this->addChild(bg);
    this->showFromTop = true;
    
    
    _cur = CurriculumManager::getInstance()->findCurriculum(category, level);

    
    string panelFilename = category=='L' ? "daily_window_title_panel_english_.png" : "daily_window_title_panel_math.png";
    if (level==0) panelFilename = "daily_window_title_panel_prek.png";
    
    auto panel = Sprite::create("MainScene/DaySelect/"+panelFilename);
    panel->setPosition(winSize.width/2, winSize.height-320);
    this->addChild(panel);
    
    auto panelLabel = TodoUtil::createLabel(_cur->levelTitle, 70, Size::ZERO, curlyFont, Color4B(255, 252, 236, 255));
    panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    panelLabel->setPosition(panel->getContentSize()/2);
    panel->addChild(panelLabel);
    
    
}


Node* createEgg(char category, int level, Size winSize)
{
    
    
    string eggName;
    eggName = ((category=='L') ? "popup_egg_english_" : "popup_egg_math_") + TodoUtil::itos(level);
    auto egg = Sprite::create("EggQuizLiteracy/Popup/"+eggName+".png");
    egg->setPosition(winSize.width/2, winSize.height-750);
    
    return egg;
    
    
}

Node* createEggFailed(char category, int level, Size winSize)
{

    
    string eggName;
    eggName = ((category=='L') ? "pretest_fail_english" : "pretest_fail_math") + TodoUtil::itos(level);
    auto egg = Sprite::create("EggQuizLiteracy/Popup/"+eggName+".png");
    egg->setPosition(winSize.width/2, winSize.height-750);
    
    return egg;
    

}



Node* createBird(char category, int level, Size winSize)
{
    string name;
    if (category=='L') name = StringUtils::format("popup_english%d_window_bird", level);
    else name = StringUtils::format("popup_math%d_window_reptile", level);
    auto bird = Sprite::create("EggQuizLiteracy/Popup/"+name+".png");
    bird->setPosition(winSize.width/2, winSize.height-750);
    
    return bird;
}



Node* createBirdFailed(char category, int level, Size winSize)
{
    
    
    string eggName;
    eggName = ((category=='L') ? "endtest_fail_english_" : "endtest_fail_math_") + TodoUtil::itos(level);
    auto egg = Sprite::create("EggQuizLiteracy/Popup/"+eggName+".png");
    egg->setPosition(winSize.width/2, winSize.height-750);
    
    return egg;
    
    
}


Node* createGlow()
{
    
    Node *node = Node::create();
    
    {
        auto sp = Sprite::create("EggQuizLiteracy/Popup/popup_window_glow_toleft.png");
        node->addChild(sp);
        
        sp->runAction(RepeatForever::create(RotateBy::create(0.5, 30)));

    }
    
    {
        auto sp = Sprite::create("EggQuizLiteracy/Popup/popup_window_glow_toright.png");
        node->addChild(sp);

        sp->runAction(RepeatForever::create(RotateBy::create(0.5, -30)));
    }
    

    return node;

    
    
}



Button* createButton(string prefix, string text) {
    auto btn = Button::create(prefix+"_normal.png", prefix+"_active.png");
    
    
    if (text.length()>0) {
        auto localTxt = LanguageManager::getInstance()->getLocalizedString(text);
        auto l = TodoUtil::createLabel(localTxt, 60, Size::ZERO, curlyFont, Color4B(255, 252, 236, 255));
        auto lPos = btn->getContentSize()/2 + Size(0, 10);
        l->setPosition(lPos);
        btn->addChild(l);
        
        btn->addTouchEventListener([btn, lPos, l](Ref*,Widget::TouchEventType) {
            if (btn->isHighlighted()) l->setPosition(lPos + Size(0, -10));
            else l->setPosition(lPos);
        });
    }
    
    return btn;
}


void EggQuizPopup::setConfirmQuit(char category, int level)
{
    
    setCommon(category, level);
    
    auto winSize = getContentSize();
    
    
    this->addChild(createEgg(category, level, winSize));
    
    
    
    {
        auto btn = createButton("EggQuizLiteracy/Popup/popup_button_test_exit", "Stop the test");
        
        btn->addClickEventListener([this](Ref*){
            this->dismiss(true);
            TodoSchoolBackButton::popGameScene();
        });
        
        btn->setPosition(Vec2(winSize.width/2, winSize.height-1130));
        this->addChild(btn);
        

    }
    
    {
        auto btn = createButton("EggQuizLiteracy/Popup/popup_button_test_backtotest", "Go back to test");
  
        btn->addClickEventListener([this](Ref*){
            this->dismiss(true);
        });
        
        btn->setPosition(Vec2(winSize.width/2, winSize.height-1320));
        this->addChild(btn);
        
    }
}

void EggQuizPopup::setPreCompleted(char category, int level)
{
    setCommon(category, level+1);
    
    auto winSize = getContentSize();
    

    
    {
        auto sp = Sprite::create("EggQuizLiteracy/Popup/popup_window_ribbon_back.png");
        sp->setPosition(winSize.width/2, winSize.height-750);
        this->addChild(sp);
        
    }
    
    

    {
        auto glow = createGlow();
        glow->setPosition(winSize.width/2, winSize.height-750);
        this->addChild(glow);
    }

    
    auto egg = createEgg(category, level+1, winSize);
    this->addChild(egg);
    
    auto eggSize = egg->getContentSize();
    
    const auto blinkTime = 0.3;
   
    {
        auto sparkle = Sprite::create("CoopScene/main_effect_sparkle_3.png");
        egg->addChild(sparkle);
        sparkle->setPosition(eggSize.width/2 + random(-200.f, 200.f), eggSize.height-250 + random(-150.f, 150.f));
        sparkle->setScale(0);
        sparkle->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
    }
    
    {
        auto sparkle = Sprite::create("CoopScene/main_effect_sparkle_3.png");
        egg->addChild(sparkle);
        sparkle->setPosition(eggSize.width/2 + random(-200.f, 200.f), eggSize.height-250 + random(-150.f, 150.f));
        sparkle->setScale(0);
        sparkle->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
    }
    
    {
        auto sparkle = Sprite::create("CoopScene/main_effect_sparkle_3.png");
        egg->addChild(sparkle);
        sparkle->setPosition(eggSize.width/2 + random(-200.f, 200.f), eggSize.height-250 + random(-150.f, 150.f));
        sparkle->setScale(0);
        sparkle->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
    }
    
    {
        auto sp = Sprite::create("EggQuizLiteracy/Popup/popup_window_ribbon_front.png");
        sp->setPosition(winSize.width/2, winSize.height-750);
        this->addChild(sp);
        
    }
    
    
    {
        auto localTxt = LanguageManager::getInstance()->getLocalizedString("Success!");
        auto l = TodoUtil::createLabel(localTxt, 70, Size::ZERO, curlyFont, Color4B(255, 252, 236, 225));
        l->setPosition(Vec2(winSize.width/2, winSize.height-965));
        this->addChild(l);
    }
    
    {
        auto s = LanguageManager::getInstance()->getLocalizedString("You passed!");
        
        auto l = TodoUtil::createLabel(s, 50, Size::ZERO, curlyFont, Color4B(115, 61, 19, 225));
        l->setPosition(Vec2(winSize.width/2, winSize.height-1130));
        this->addChild(l);
    }
    
    {
        auto btn = createButton("EggQuizLiteracy/Popup/popup_window_check", "");
        
        btn->addClickEventListener([this](Ref*){
            this->dismiss(true);

        });
        
        btn->setPosition(Vec2(winSize.width/2, winSize.height-1320));
        this->addChild(btn);
        
    }
    
    
}


void EggQuizPopup::setPostCompleted(char category, int level)
{

    _cur = CurriculumManager::getInstance()->findCurriculum(category, level);
    UserManager::getInstance()->setPretestProgressType(_cur->levelID, PretestProgressType::pass);
    
    auto winSize = getContentSize();
    
    Point eggCenter = Point(winSize.width/2, winSize.height-750-150);
    
    {
        auto glow = createGlow();
        glow->setPosition(eggCenter);
        this->addChild(glow);
    }
    
    

    
    {
        for (int i=0; i<60; i++) {
            auto file = StringUtils::format("EggQuizLiteracy/Popup/popup_effect_confetti_%d.png", random(1, 8));
            auto sp = Sprite::create(file);
            auto posNode = Node::create();
            
            posNode->addChild(sp);
            
            posNode->setPosition(eggCenter);
            
            auto rotateAction = RotateBy::create(0.3, Vec3(random(-30.f, 30.f), random(-30.f, 30.f), random(-30.f, 30.f)));
            sp->runAction(RepeatForever::create(rotateAction));
            
            auto pos = Vec2(random<float>(winSize.width*0.15, winSize.width*0.85), random(winSize.height*0.4, winSize.height*0.9));
            auto speed = random(150.f, 250.f);
            float delay = random(0.0, 0.2);
            posNode->runAction(Sequence::create(DelayTime::create(delay), EaseOut::create(MoveTo::create(random(0.3, 0.5), pos), 2.0), MoveTo::create(pos.y / speed, Vec2(pos.x, -100)), nullptr));
            
            this->addChild(posNode);
            
            
        }
        
        
    }
    
    {
        auto b = createBird(category, level, winSize);
        b->setPosition(eggCenter);
        this->addChild(b);
    }
    
 
    
    
    {
        auto localTxt = LanguageManager::getInstance()->getLocalizedString("Congratulations!");
        auto l = TodoUtil::createLabel(localTxt, 60, Size::ZERO, curlyFont, Color4B(255, 252, 236, 225));
        l->setPosition(Vec2(winSize.width/2, winSize.height-965-175));
        this->addChild(l);
    }

    
    {
        auto btn = createButton("EggQuizLiteracy/Popup/popup_window_check", "");
        
        btn->addClickEventListener([this](Ref*){
            this->dismiss(true);
        });
        
        btn->setPosition(Vec2(winSize.width/2, winSize.height-1375));
        this->addChild(btn);
        
    }
    
    
}


void EggQuizPopup::setPreFailed(char category, int level)
{
    
    setCommon(category, level+1);
    
    auto winSize = getContentSize();
    
    
    
    {
        auto b = createEggFailed(category, level+1, winSize);
        b->setPosition(winSize.width/2, winSize.height-750);
        this->addChild(b);
    }
    
    
    
    {
        auto s1 = LanguageManager::getInstance()->getLocalizedString("You are not ready.");
        auto s2 = LanguageManager::getInstance()->getLocalizedString("Practice more and try again later.");
        
        auto l = TodoUtil::createLabel(s1+"\n"+s2, 50, Size::ZERO, curlyFont, Color4B(115, 61, 19, 225), TextHAlignment::CENTER);
        l->setPosition(Vec2(winSize.width/2, winSize.height-1120));
        this->addChild(l);
    }
    
    {
        auto btn = createButton("EggQuizLiteracy/Popup/pretest_button_back", "");
        
        btn->addClickEventListener([this](Ref*){
            this->dismiss(true);
            
        });
        
        btn->setPosition(Vec2(winSize.width/2, winSize.height-1320));
        this->addChild(btn);
        
    }

    
    
}


void EggQuizPopup::setPostFailed(char category, int level)
{
    setCommon(category, level);
    
    auto winSize = getContentSize();
    
    UserManager::getInstance()->setPretestProgressType(_cur->levelID, PretestProgressType::fail);
    
    {
        auto b = createBirdFailed(category, level, winSize);
        b->setPosition(winSize.width/2, winSize.height-750);
        this->addChild(b);
    }
    
    
    
    {
        auto s1 = LanguageManager::getInstance()->getLocalizedString("You are not ready.");
        auto s2 = LanguageManager::getInstance()->getLocalizedString("Practice more and try again later.");
        auto l = TodoUtil::createLabel(s1+"\n"+s2, 50, Size::ZERO, curlyFont, Color4B(115, 61, 19, 225), TextHAlignment::CENTER);
        l->setPosition(Vec2(winSize.width/2, winSize.height-1120));
        this->addChild(l);
    }
    
    {
        auto btn = createButton("EggQuizLiteracy/Popup/pretest_button_back", "");
        
        btn->addClickEventListener([this](Ref*){
            this->dismiss(true);
            
        });
        
        btn->setPosition(Vec2(winSize.width/2, winSize.height-1320));
        this->addChild(btn);
        
    }
}

