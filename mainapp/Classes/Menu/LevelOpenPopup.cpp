//
//  LevelOpenPopup.cpp
//  KitkitSchool
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
#include "Managers/GameSoundManager.h"
#include "CoopScene.hpp"
#include "Fish.hpp"

#include <time.h>
#include <algorithm>
#include <iterator>

namespace LevelOpenPopupSpace {
    
    
    const Size popupSize = Size(1626,1210);
    const string folder = "CoopScene/LevelOpenPopup/";
    const string hindiFontName = "fonts/chanakya.ttf";
    const string fontName = "fonts/mukta-bold.ttf";
    
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
    string languageCode = LanguageManager::getInstance()->getCurrentLanguageCode();
    auto cur = CurriculumManager::getInstance()->findCurriculum(levelID);
    
    auto bg = Sprite::create(folder+"popup_pretest_bg.png");
    bg->setAnchorPoint(Vec2::ZERO);
    addChild(bg);
    
    
    string eggName;
    
    if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
        eggName = "pretest_image_thumbnail_fishbowl";
        
    } else {
        eggName = "todoschool_pretest_egg_";
        eggName += (cur->category=='L') ? "english_" : "math_";
        eggName += TodoUtil::itos(cur->categoryLevel);
        
    }
    
    auto egg = Sprite::create(folder + eggName + ".png");
    egg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    egg->setPosition(Vec2(30, 906));
    bg->addChild(egg);

    
    string panelName = "todoschool_pretest_panel_";
//    if (cur->categoryLevel==0) panelName += "prek.png";
    if (cur->category=='L') panelName += "english.png";
    else panelName += "math.png";
    auto panel = Sprite::create(folder+panelName);
    panel->setPosition(popupSize.width/2, popupSize.height-100);
    bg->addChild(panel);

    string levelTitle;
    if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
        int levelIndex = UserManager::getInstance()->getFishPresentCurrentProgressLevel(levelID);
        auto curriculum = CurriculumManager::getInstance()->findCurriculum(cur->category, levelIndex);
        if (curriculum) {
            levelTitle = curriculum->levelTitle;
        }
    } else {
        levelTitle = cur->levelTitle;
    }
    
    auto panelLabel = TodoUtil::createLabel(levelTitle, 60, Size::ZERO, hindiFontName, Color4B(255, 249, 235, 255));
    panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    panelLabel->setPosition(panel->getContentSize()/2+Size(0, -5));
    panel->addChild(panelLabel);
    
    
    auto desc = Sprite::create(folder+"todoschool_pretest_saying.png");
    desc->setPosition(Vec2(202, 748));
    desc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->addChild(desc);
    
    
    string s1;
    if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
        s1 = LanguageManager::getInstance()->getLocalizedString("Take the quiz to add me to your sea world!");
    } else {
        s1 = LanguageManager::getInstance()->getLocalizedString("Do you want to take a test on this egg?");
    }

    /*
     string s2;
    if (cur->categoryLevel==1) s2 = "Welcome!";
    else if (cur->categoryLevel<=3) s2 = "Prove it!";
    else s2 = "Try and get 8 questions correct!";
    s2 = LanguageManager::getInstance()->getLocalizedString(s2);
    */
    {
        auto descLabel = TodoUtil::createLabel(s1, 50, Size::ZERO, hindiFontName, Color4B(65, 5, 5, 255), TextHAlignment::CENTER);
        descLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        descLabel->setPosition(desc->getContentSize()/2+Size(50, -30));
        desc->addChild(descLabel);
    }
    
    /*
    {
        auto descLabel = TodoUtil::createLabel(s2, 50, Size::ZERO, fontName, Color4B(65, 5, 5, 255), TextHAlignment::CENTER);
        descLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        descLabel->setPosition(desc->getContentSize()/2+Size(50, -30-35));
        desc->addChild(descLabel);
    }
     */
    
    if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
        auto testPanel = Sprite::create(folder+"pretest-popup_image_paper.png");
        testPanel->setPosition(bg->getContentSize() / 2 + Size(0, -70));
        bg->addChild(testPanel);
        
        int levelIndex = UserManager::getInstance()->getFishPresentCurrentProgressLevel(levelID);
        auto fishNode = Fish::create(cur->category, levelIndex + 1, false);
        fishNode->setContentSize(Size(528, 281));
        fishNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        fishNode->setPosition(Vec2(testPanel->getContentSize().width / 2, testPanel->getContentSize().height - 50));
        testPanel->addChild(fishNode);
        
    } else {
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
            prefix = StringUtils::format("%s/L%d_", languageCode.c_str(), cur->categoryLevel);
        } else {
            prefix = StringUtils::format("math/M%d_", cur->categoryLevel);
        }
        
        float thumbTargetWidth = 300;
        
        for (int i=0; i<numSS; i++) {
            auto testPanel = Sprite::create(folder+"popup_pretest_screenshotbg.png");
            testPanel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            testPanel->setPosition(testX, 330);
            bg->addChild(testPanel);
            
            auto SS = Sprite::create(folder + "ScreenShot/" + prefix + StringUtils::format("%d.png", i+1));
            if (SS) {
                if (SS->getContentSize().width > thumbTargetWidth) SS->setScale(thumbTargetWidth/SS->getContentSize().width);
                SS->setPosition(testPanel->getContentSize().width/2, testPanel->getContentSize().height/2-14);
                testPanel->addChild(SS);
            }
            
            testX+= testStep;
        }
    
    }
    
    
    auto startBtn = ui::Button::create();
    auto cancelBtn = ui::Button::create();
    
    if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
        Color4B defaultColor;
        bool isEnable = UserManager::getInstance()->getStars() >= 10;
        if(isEnable) {
            startBtn->loadTextures(folder+"pretest-popup_button_normal.png", folder+"pretest-popup_button_touch.png");
            defaultColor = Color4B(255, 249, 235, 255);
            
        } else {
            startBtn->loadTextures(folder+"pretest-popup_button_disabled.png", folder+"pretest-popup_button_disabled.png");
            defaultColor = Color4B(190, 249, 249, 249);
        }
        
        auto startL = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString("Start"), 75, Size::ZERO, hindiFontName, defaultColor);
        auto startLPos = Vec2(startBtn->getContentSize()/2 + Size(0, 35));
        
        auto coinL = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString("10"), 50, Size::ZERO, hindiFontName, isEnable ? defaultColor : Color4B(190, 240, 76, 31));
        
        auto coinLPos = Vec2(startBtn->getContentSize()/2 + Size(20, -40));
        
        startL->setPosition(startLPos);
        startBtn->addChild(startL);
        
        coinL->setPosition(coinLPos);
        startBtn->addChild(coinL);
        
        if(isEnable) {
            startBtn->addTouchEventListener([startBtn, startL, coinL, defaultColor](Ref*, ui::Widget::TouchEventType e){
                if (startBtn->isHighlighted()) {
                    startL->setTextColor(Color4B(255, 179, 48, 255));
                    coinL->setTextColor(Color4B(255, 175, 48, 255));
                    
                } else {
                    startL->setTextColor(defaultColor);
                    coinL->setTextColor(defaultColor);
                }
            });

        } else {
            startBtn->setOpacity(190);
            startBtn->setEnabled(false);
        }

    } else {
        startBtn->loadTextures(folder+"main_button_normal.png", folder+"main_button_active.png");
        auto startL = TodoUtil::createLabel(LanguageManager::getInstance()->getLocalizedString("Start"), 70, Size::ZERO, hindiFontName, Color4B(255, 249, 235, 255));
        auto startLPos = Vec2(startBtn->getContentSize()/2 + Size(0, -2));
        
        startL->setPosition(startLPos);
        startBtn->addChild(startL);
    }
    
    startBtn->setZoomScale(0);
    startBtn->addClickEventListener([this, cur, startBtn, cancelBtn](Ref*) {
        GameSoundManager::getInstance()->stopAllEffects();

        SoundEffect::buttonEffect().play();
        if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
            if (onRemoveCoin) {
                startBtn->setTouchEnabled(false);
                cancelBtn->setTouchEnabled(false);
                auto worldPos = startBtn->convertToWorldSpace(startBtn->getContentSize() / 2);
                onRemoveCoin(worldPos);
            }
        } else {
            if (onOpenLevel) {
                onOpenLevel();
            }
            this->dismiss(true);
        }

        /*
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
         */
    });
    startBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    startBtn->setPosition(Vec2(popupSize.width/2, 114));
    bg->addChild(startBtn);
    
    cancelBtn->loadTextures(folder+"todoschool_pretest_button_close_normal.png", folder+"todoschool_pretest_button_close_active.png");
    cancelBtn->setZoomScale(0);
                            
    cancelBtn->addClickEventListener([this](Ref*) {
        GameSoundManager::getInstance()->stopAllEffects();
        this->dismiss(true);
    });
    cancelBtn->setPosition(Vec2(1522, 1070));
    bg->addChild(cancelBtn);
    
    if (cur->categoryLevel == CoopScene::LEVEL_FISH_PRESENT) {
        GameSoundManager::getInstance()->playEffectSoundForAutoStart(folder + "Audio/" + StringUtils::format("%s/", languageCode.c_str())+"takequiztoaddmeseaworld.m4a");
    } else {
        GameSoundManager::getInstance()->playEffectSoundForAutoStart(folder + "Audio/" + StringUtils::format("%s/", languageCode.c_str())+"guide.m4a");
    }
}
