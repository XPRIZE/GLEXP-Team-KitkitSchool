//
//  CoopScene.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/20/16.
//
//

#include "CoopScene.hpp"
#include "Bird.hpp"
#include "LevelOpenPopup.hpp"
#include "DaySelectPopup.hpp"

#include "CustomDirector.h"
#include "MainScene2.hpp"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/GameSoundManager.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"


#include "Utils/TodoUtil.h"

namespace CoopSceneSpace {
    const Size coopSize = Size(2560, 1800);
    const float birdScale = 0.7;
    

    const int glowZ = 2000;
    const int zoomZ = 3000;
    
    

};

using namespace CoopSceneSpace;

Scene* CoopScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = CoopScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool CoopScene::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    setTouchEnabled(true);
    
    auto dirSize = Director::getInstance()->getWinSize();
    
    float coopScale = MIN(dirSize.width / coopSize.width,
                          dirSize.height / coopSize.height);
    
    auto bg = Sprite::create("CoopScene/coop_bg.jpg");
    bg->setScale(coopScale);
    bg->setPosition(dirSize/2);
    addChild(bg);
    
    _coopView = Node::create();
    _coopView->setContentSize(coopSize);
    _coopView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    

    _coopView->setScale(coopScale);
    _coopView->setPosition(dirSize/2.f);
    addChild(_coopView);


    createBirds();
    
    
    _currentZ = 100;
    
    
    const Size debugViewSize = Size(dirSize.width-60, 200);
    _debugView = LayerColor::create(Color4B(0, 0, 0, 64), debugViewSize.width, debugViewSize.height);
    _debugView->setPosition(30, dirSize.height-debugViewSize.height);
    _debugView->setContentSize(dirSize);
    addChild(_debugView, _currentZ++);
    
    
    
    bool isDebug = UserManager::getInstance()->isDebugMode();
    bool isGameTesting = UserManager::getInstance()->isGameTestingMode();
    
    
    
    
    if (!isDebug) {
        _debugView->setVisible(false);
    } else {
    
        Label* versionTitle = Label::createWithTTF("Version : "+UserManager::getInstance()->getAppVersion(), "fonts/OpenSans-Bold.ttf", 50);
        versionTitle->setTextColor(Color4B::WHITE);
        versionTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        versionTitle->setPosition(Vec2(30, debugViewSize.height/2));
        _debugView->addChild(versionTitle);
        
        
    
        ui::Button* resetButton = ui::Button::create();
        resetButton->setTitleText("reset");
        resetButton->setTitleFontSize(50);
        resetButton->setPosition(Vec2(debugViewSize.width-200, debugViewSize.height/2));
        resetButton->addTouchEventListener([](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                UserManager::getInstance()->resetStatus();
                Director::getInstance()->popToRootScene();
            }
        }  );
        _debugView->addChild(resetButton);
    
        
        {
            ui::Button* debugModeButton = ui::Button::create();
            auto txt = isDebug ? "debug: on" : "debug: off";
            debugModeButton->setTitleText(txt);
            debugModeButton->setTitleFontSize(50);
            debugModeButton->setPosition(Vec2(debugViewSize.width-500, debugViewSize.height/2));
            debugModeButton->addTouchEventListener([debugModeButton, this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    auto debug = !(UserManager::getInstance()->isDebugMode());
                    auto txt = debug ? "debug: on" : "debug: off";
                    debugModeButton->setTitleText(txt);
                    UserManager::getInstance()->setDebugMode(debug);
                    if (!debug) {
                        
                        _debugView->setVisible(false);
                        //UserManager::getInstance()->setGameTestingMode(false);
                    }
                    
                }
            }  );
            _debugView->addChild(debugModeButton);
        }
        
        {
            ui::Button* gameTestingButton = ui::Button::create();
            auto txt = isGameTesting ? "gameTesting: on" : "gameTesting: off";
            gameTestingButton->setTitleText(txt);
            gameTestingButton->setTitleFontSize(50);
            gameTestingButton->setPosition(Vec2(debugViewSize.width-1000, debugViewSize.height/2));
            gameTestingButton->addTouchEventListener([gameTestingButton](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    auto gameTesting = !(UserManager::getInstance()->isGameTestingMode());
                    auto txt = gameTesting ? "gameTesting: on" : "gameTesting: off";
                    gameTestingButton->setTitleText(txt);
                    UserManager::getInstance()->setGameTestingMode(gameTesting);
                    
                }
            }  );
            _debugView->addChild(gameTestingButton);
        }
        
        {
            ui::Button* langButton = ui::Button::create();
            auto langStr = LanguageManager::getInstance()->getCurrentLanguageString();
            
            auto txt = "Lang: " + langStr;
            langButton->setTitleText(txt);
            langButton->setTitleFontSize(50);
            langButton->setPosition(Vec2(debugViewSize.width-1500, debugViewSize.height/2));
            langButton->addTouchEventListener([langButton, this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    auto lang = LanguageManager::getInstance()->getCurrentLanguage();
                    LanguageManager::LanguageType newLang;
                    
                    if (lang==LanguageManager::ENGLISH) {
                        newLang = LanguageManager::SWAHILI;
                    } else {
                        newLang = LanguageManager::ENGLISH;
                    }
                    LanguageManager::getInstance()->setCurrentLanguage(newLang);
                    
                    auto langStr = LanguageManager::getInstance()->getCurrentLanguageString();
                    auto txt = "Lang: " + langStr;
                    langButton->setTitleText(txt);
                    
                    this->createBirds();
                
                }
            }  );
            
            _debugView->addChild(langButton);

        }
    }

//    std::string langTitle;
//    switch (LanguageManager::getInstance()->getCurrentLanguage()) {
//        case LanguageManager::ENGLISH:
//            langTitle = "English";
//            break;
//            
//        case LanguageManager::SWAHILI:
//            langTitle = "Swahili";
//            break;
//        default:
//            break;
//    }
//    
//    _langButton = ui::Button::create();
//    _langButton->setTitleText(langTitle);
//    _langButton->setTitleFontSize(50);
//    _langButton->setTitleColor(Color3B::BLACK);
//    _langButton->setPosition(Vec2(200, visibleSize.height-100));
//    _langButton->addTouchEventListener(CC_CALLBACK_2(CoopScene::onLanguageTouched, this));
//    addChild(_langButton);
    

    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, dirSize.height-25));
    addChild(backButton);

    return true;
}



void CoopScene::createBirds()
{
    
    
    _coopView->removeAllChildren();
    
    birds.clear();
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    
    for (auto it : CurriculumManager::getInstance()->levels) {
        LevelCurriculum cur = it.second;
        if (cur.lang!=lang) continue;
        
        bool isOpen = UserManager::getInstance()->isLevelOpen(cur.levelID);
        if (cur.categoryLevel==0 && !isOpen) {
            UserManager::getInstance()->setLevelOpen(cur.levelID);
            isOpen = true;
        }
        
        
        Bird* bird = Bird::create(cur.category, cur.categoryLevel, cur.levelID);

        
        
        
        
        int x = cur.category=='L' ? 1-(cur.categoryLevel%2) : 2+(cur.categoryLevel%2);
        int y = 2-(cur.categoryLevel/2);
        Point p = Point(x, y);
        
        Vec2 panelPos = Vec2(coopSize.width/8.f*(1+2*p.x), 545*p.y);
        bird->setPosition(panelPos+Vec2(0, 120));
        if (isOpen) {
            bird->setScale(birdScale);
            bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
            bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(cur.levelID));
        } else {
            bird->setStatus(Bird::BirdStatus::EGG);
        }
        
        
        string nestName = (cur.category=='M') ? "coop_math_nest.png" : "coop_english_nest.png";
        auto nest = Sprite::create("CoopScene/"+nestName);
        nest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        nest->setPosition(panelPos + Vec2(0, 20));
        _coopView->addChild(nest, _currentZ++);
        
        Sprite *shadow;
        if (isOpen) {
            shadow = Sprite::create("CoopScene/coop_bird_shadow.png");
            shadow->setScale(bird->getBoundingBox().size.width / shadow->getContentSize().width);
        } else {
            shadow = Sprite::create("CoopScene/coop_egg_shadow.png");
        }
        
        shadow->setPosition(bird->getPosition());
        _coopView->addChild(shadow, _currentZ++, bird->getType());
        
        
        
        _coopView->addChild(bird, _currentZ++);
        birds.push_back(bird);
        

        
        
        string panelFilename = cur.category=='L' ? "coop_woodpanel_english.png" : "coop_woodpanel_math.png";
        if (cur.categoryLevel==0) panelFilename = "coop_woodpanel_prek.png";
        auto panel = Sprite::create("CoopScene/"+panelFilename);
        panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        panel->setPosition(panelPos);
        _coopView->addChild(panel, _currentZ++);
        
        auto panelLabel = TodoUtil::createLabel(cur.levelTitle, 42, Size::ZERO, "fonts/TodoMainCurly.ttf", Color4B(255, 252, 236, 255));
        panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        panelLabel->setPosition(panel->getContentSize()/2+Size(0, -5));
        panel->addChild(panelLabel);
    }
    
    
    for (auto bird : birds) {
        bird->onTouchBegan = [bird, this](){
            if (!isTouchEnabled()) return;
            bird->runTouchAnimation();

            
        };
        
        bird->onTouchEnded = [this, bird]() {
            
            
            if (!isTouchEnabled()) return;
            setTouchEnabled(false);
            
            auto c = bird->getCategory();
            auto l = bird->getCategoryLevel();
            auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
            
            auto cur = CurriculumManager::getInstance()->findCurriculum(c, l);
            if (cur) {
                auto levelID = CurriculumManager::getInstance()->makeLevelID(lang, c, l);
                
                if (UserManager::getInstance()->isLevelOpen(levelID)) {

                    
                    this->showDaySelectPopup(levelID);
                    
                } else {
                    auto popup = LevelOpenPopup::create(this);
                    popup->setup(levelID);
                    popup->onloadCallback = CallFunc::create([this](){
                        this->setTouchEnabled(true);
                    });
                    
                    popup->show(this, true);
                    popup->onOpenLevel = [this, levelID, bird](){
                        UserManager::getInstance()->setLevelOpen(levelID);
                        this->hatchEgg(bird);
                        
                    };
                    
                    
                }
            } else {
                setTouchEnabled(true);
            }
            
            
            
            
        };
        
        
        
        
    }
    

}

void CoopScene::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    GameSoundManager::getInstance()->playBGM("Common/Music/SFX_AllAnimals_IncAmbience.m4a");
    
    checkLevelClear();
}

void CoopScene::hatchEgg(Bird *bird)
{
    
    
    setTouchEnabled(false);
    
    float zoomTime = 1.0;
    const float zoomRate = 3.0;
    auto orgZ = bird->getLocalZOrder();
    _coopView->reorderChild(bird, zoomZ);
    auto shadow = _coopView->getChildByTag(bird->getType());
    shadow->removeFromParent();
    
    
    auto orgPos = bird->getPosition();
    auto zoomPos = Vec2(coopSize.width/2, coopSize.height/2-350);
    auto spawn1 = Spawn::create(EaseIn::create(MoveTo::create(zoomTime, zoomPos), 3.0), EaseIn::create(ScaleTo::create(zoomTime, birdScale*zoomRate), 3.0), nullptr);
    auto spawn2 = Spawn::create(EaseIn::create(MoveTo::create(zoomTime, orgPos), 3.0), EaseIn::create(ScaleTo::create(zoomTime, birdScale), 3.0), nullptr);
    
    auto glow = Node::create();
    glow->setContentSize(coopSize);
    glow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    glow->setPosition(coopSize/2);
    glow->setScale(0);
    glow->setVisible(false);
    _coopView->addChild(glow, glowZ);
    
    auto g2 = Sprite::create("CoopScene/effect_egg_glow_2.png");
    g2->setPosition(coopSize/2);
    glow->addChild(g2);
    
    auto g3 = Sprite::create("CoopScene/effect_egg_glow_3.png");
    g3->setPosition(coopSize/2);
    glow->addChild(g3);
    
    auto g1 = Sprite::create("CoopScene/effect_egg_glow_1.png");
    g1->setPosition(coopSize/2);
    glow->addChild(g1);
    
    
    auto sparkle = Node::create();
    sparkle->setVisible(false);
    bird->addChild(sparkle);
    auto s1 = Sprite::create("CoopScene/main_effect_sparkle_3.png");
    sparkle->addChild(s1);
    
    auto s2 = Sprite::create("CoopScene/main_effect_sparkle_3.png");
    sparkle->addChild(s2);
    
    auto s3 = Sprite::create("CoopScene/main_effect_sparkle_3.png");
    sparkle->addChild(s3);
    
    auto s4 = Sprite::create("CoopScene/new_creature_effect.png");
    sparkle->addChild(s4);
    
    auto seq = Sequence::create(spawn1, CallFunc::create([this, bird, glow, g2, g3](){
        
        
        bird->loadAnimation();
        bird->runCrackAnimation();
        SoundEffect::eggCrackEffect().play();
        
        
        glow->setVisible(true);
        
        g2->runAction(RepeatForever::create(RotateBy::create(0.5, 30)));
        g3->runAction(RepeatForever::create(RotateBy::create(0.5, -30)));
        
        auto glowSeq = Sequence::create(DelayTime::create(1.0),
                                        CallFunc::create([](){SoundEffect::eggHaloEffect().play();}),
                                        ScaleTo::create(1.0, 1.0),
                                        DelayTime::create(2.0),
                                        ScaleTo::create(1.0, 0.0),
                                        CallFunc::create([glow](){glow->removeFromParent();}), nullptr);
        
        glow->runAction(glowSeq);
        
        
        
    }), DelayTime::create(2.0),
        FadeOut::create(1.0),
        CallFunc::create([this, bird, zoomRate, sparkle, s1, s2, s3, s4](){
        
        bird->setScale(birdScale*zoomRate);
        auto cur = CurriculumManager::getInstance()->findCurriculum(bird->getCategory(), bird->getCategoryLevel());
        bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
        bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(cur->levelID));

        bird->setPosition(bird->getPosition()+Vec2(0, 100));

        
        
        auto size = bird->getContentSize();
        

        sparkle->setVisible(true);
        sparkle->setContentSize(size);

        
        const auto blinkTime = 0.3;
        

        s1->setPosition(random(0.f, size.width), random(size.height*0.6f, size.height));
        s1->setScale(0);
        s1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));

        s2->setPosition(random(0.f, size.width*0.3f), random(0.f, size.height*0.3f));
        s2->setScale(0);
        s2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
        

        s3->setPosition(random(size.width*0.7f, size.width), random(0.f, size.height*0.3f));
        s3->setScale(0);
        s3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
        
        s4->setPosition(size/2);
        s4->setScale(1.1);
        s4->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(blinkTime*2.3, 0.9), ScaleTo::create(blinkTime*2.3, 1.1), nullptr)));
        
        
    }), DelayTime::create(1.0),
        CallFunc::create([](){SoundEffect::eggHatchEffect().play();}),
        FadeIn::create(0.5),
        DelayTime::create(1.0),
        spawn2,
        CallFunc::create([this, bird, orgZ](){
        SoundEffect::particleEffect().play();
        auto shadow = Sprite::create("CoopScene/coop_bird_shadow.png");
        shadow->setScale(bird->getBoundingBox().size.width / shadow->getContentSize().width);
        shadow->setPosition(bird->getPosition());
        _coopView->addChild(shadow, orgZ, bird->getType());
        
        _coopView->reorderChild(bird, orgZ);
        
        setTouchEnabled(true);
        
    }), nullptr);
    
    bird->runAction(seq);
    
    

}


void CoopScene::showDaySelectPopup(std::string levelID)
{
    if (UserManager::getInstance()->isGameTestingMode()) {
        UserManager::getInstance()->setCurrentLevelID(levelID);
        ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
        return;
    }
    
    
    auto popup = DaySelectPopup::create(this, levelID);
    popup->show();
    
    popup->onSelectDay = [this, levelID](int day) {
        UserManager::getInstance()->setCurrentLevelID(levelID);
        UserManager::getInstance()->setCurrentDay(day);
        
        ((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
    };
    
    popup->onDismiss = [this]() {
        setTouchEnabled(true);
        GameSoundManager::getInstance()->playBGM("Common/Music/SFX_AllAnimals_IncAmbience.m4a");
        
    };
    
    /*
    popup->onSelectDay = [this, levelID](int day) {
        UserManager::getInstance()->setCurrentLevelID(levelID);
        UserManager::getInstance()->setCurrentDay(day);
        
        addDayGameButtons(levelID, day, gameIcons);
        
        if (!UserManager::getInstance()->isDayCleared(levelID, day)) {
            spawnGameEggs(0.3);
        }
        
    };
   */
    
    
}

void CoopScene::checkLevelClear()
{
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto u = UserManager::getInstance();
    for (auto bird : birds) {
        if (bird->getStatus() == Bird::BirdStatus::EGG_HATCHED) continue;
        auto cl = bird->getCategoryLevel();
        if (cl==0) continue;
        
        auto previous = CurriculumManager::getInstance()->makeLevelID(lang, bird->getCategory(), cl-1);
        
        if (u->isLevelCleared(previous)) {
            u->setLevelOpen(bird->getLevelID());
            this->hatchEgg(bird);
            return;
        }
        
    }
    
}
