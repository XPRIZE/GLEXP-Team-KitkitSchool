//
//  LevelOpenPopup.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 12/25/16.
//
//

#include "CCAppController.hpp"

#include "LevelOpenPopup.hpp"
#include "Utils/TodoUtil.h"
#include "3rdParty/CCNativeAlert.h"

#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/LanguageManager.hpp"

#include <time.h>
#include <algorithm>
#include <iterator>

namespace LevelOpenPopupSpace {
    
    
    const Size popupSize = Size(1626,1210);
    const string folder = "CoopScene/LevelOpenPopup/";
    const string fontName = "fonts/TodoMainCurly.ttf";
    
    
}

using namespace LevelOpenPopupSpace;
using namespace std;

LevelOpenPopup* LevelOpenPopup::create(Node* parentView)
{
    LevelOpenPopup *popup = new (std::nothrow) LevelOpenPopup();
    if (popup && popup->init(parentView))
    {
        popup->autorelease();
        return popup;
    }
    CC_SAFE_DELETE(popup);
    return nullptr;
}


bool LevelOpenPopup::init(cocos2d::Node *parentView)
{
    if (!PopupBase::init(parentView, popupSize)) {
        return false;
    }
    
    return true;
}


void LevelOpenPopup::setup(std::string levelID)
{
    
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    
    auto bg = Sprite::create(folder+"popup_pretest_bg.png");
    bg->setAnchorPoint(Vec2::ZERO);
    addChild(bg);
    
    
    string eggName = "todoschool_pretest_egg_";
    eggName += (cur->category=='L') ? "english_" : "math_";
    eggName += TodoUtil::itos(cur->categoryLevel);
    auto egg = Sprite::create(folder + eggName + ".png");
    egg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    egg->setPosition(Vec2(30, 906));
    bg->addChild(egg);
    
    string panelName = "todoschool_pretest_panel_";
    if (cur->categoryLevel==0) panelName += "prek.png";
    else if (cur->category=='L') panelName += "english.png";
    else panelName += "math.png";
    auto panel = Sprite::create(folder+panelName);
    panel->setPosition(popupSize.width/2, popupSize.height-100);
    bg->addChild(panel);
    
    auto panelLabel = TodoUtil::createLabel(cur->levelTitle, 60, Size::ZERO, fontName, Color4B(255, 249, 235, 255));
    panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    panelLabel->setPosition(panel->getContentSize()/2+Size(0, -5));
    panel->addChild(panelLabel);
    
    
    auto desc = Sprite::create(folder+"todoschool_pretest_saying.png");
    desc->setPosition(Vec2(202, 748));
    desc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->addChild(desc);
    
    
    string s1 = LanguageManager::getInstance()->getLocalizedString("Are you ready for") + " " + cur->levelTitle + "?";
    string s2;
    if (cur->categoryLevel==1) s2 = "Welcome!";
    else if (cur->categoryLevel<=3) s2 = "Prove it!";
    else s2 = "Try and get 8 questions correct!";
    s2 = LanguageManager::getInstance()->getLocalizedString(s2);
    
    {
        auto descLabel = TodoUtil::createLabel(s1, 50, Size::ZERO, fontName, Color4B(65, 5, 5, 255), TextHAlignment::CENTER);
        descLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        descLabel->setPosition(desc->getContentSize()/2+Size(50, -30+35));
        desc->addChild(descLabel);
    }
    
    {
        auto descLabel = TodoUtil::createLabel(s2, 50, Size::ZERO, fontName, Color4B(65, 5, 5, 255), TextHAlignment::CENTER);
        descLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        descLabel->setPosition(desc->getContentSize()/2+Size(50, -30-35));
        desc->addChild(descLabel);
    }
    
    {
        int numSS = 3;
        if (cur->categoryLevel==1) numSS=2;
        
        const float margin = 11;
        const float SSwidth = 416;
        const float centerX = 178 + (SSwidth*3 + margin*2)/2;
        
        float width = SSwidth*numSS + margin*(numSS-1);
        float testX = centerX - width/2;
        float testStep = margin + SSwidth;
        //float testX = 178;
        //float testStep = 11 + 416;
        
        string prefix;
        
        if (cur->category=='L') {
            prefix = StringUtils::format("%s/L%d_", LanguageManager::getInstance()->getCurrentLanguageCode().c_str(), cur->categoryLevel);
        } else {
            prefix = StringUtils::format("math/M%d_", cur->categoryLevel);
        }
        
        
        for (int i=0; i<numSS; i++) {
            auto testPanel = Sprite::create(folder+"popup_pretest_screenshotbg.png");
            testPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            testPanel->setPosition(testX, 330);
            bg->addChild(testPanel);
            
            auto SS = Sprite::create(folder + "ScreenShot/" + prefix + StringUtils::format("%d.png", i+1));
            if (SS) {
                SS->setPosition(testPanel->getContentSize()/2);
                testPanel->addChild(SS);
            }
            
            testX+= testStep;
        }
    
    }
    
    
    auto startBtn = ui::Button::create();
    startBtn->loadTextures(folder+"main_button_normal.png", folder+"main_button_active.png");
    startBtn->setZoomScale(0);
    
    auto startL = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString("Start"), 70, Size::ZERO, fontName, Color4B(255, 249, 235, 255));
    

    auto startLPos = Vec2(startBtn->getContentSize()/2 + Size(0, -2));
    
    startL->setPosition(startLPos);
    startBtn->addChild(startL);
    startBtn->addTouchEventListener([startBtn, startL, startLPos](Ref*, ui::Widget::TouchEventType){
        //startL->setPosition(startBtn->isHighlighted() ? startLPos+Vec2(0, -12) : startLPos);
    });
    
    startBtn->addClickEventListener([this, cur](Ref*) {
        SoundEffect::buttonEffect().play();
        
        
        if (cur->categoryLevel==1) {
            if (onOpenLevel) {
                onOpenLevel();
            }
            this->dismiss(true);
        } else {
            CCAppController::sharedAppController()->startEggQuiz(cur->category, cur->categoryLevel-1, true, [this](bool cleared) {
                if (cleared && onOpenLevel) {
                    onOpenLevel();
                }
                this->dismiss(true);
            });
        }
    });
    startBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    startBtn->setPosition(Vec2(popupSize.width/2, 114));
    bg->addChild(startBtn);
    
    auto cancelBtn = ui::Button::create();
    cancelBtn->loadTextures(folder+"todoschool_pretest_button_close_normal.png", folder+"todoschool_pretest_button_close_active.png");
    cancelBtn->setZoomScale(0);
                            
    cancelBtn->addClickEventListener([this](Ref*) {
        this->dismiss(true);
    });
    cancelBtn->setPosition(Vec2(1522, 1070));
    bg->addChild(cancelBtn);
    
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto postBtn = ui::Button::create();
        postBtn->setTitleText("Post Test");
        postBtn->setTitleFontName(fontName);
        postBtn->setTitleColor(Color3B::WHITE);
        postBtn->setTitleFontSize(50);
        postBtn->setPosition(Vec2(popupSize.width*0.75, 114));
        bg->addChild(postBtn);
        
        postBtn->addClickEventListener([this, cur](Ref*) {
            
            CCAppController::sharedAppController()->startEggQuiz(cur->category, cur->categoryLevel, false, [this](bool cleared) {
            });
        });
    }

}
