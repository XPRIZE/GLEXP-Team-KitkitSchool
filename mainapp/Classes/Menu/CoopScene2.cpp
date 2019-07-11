//
//  CoopScene2.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 3/25/18.
//
//

#include "CoopScene2.hpp"
#include "Bird.hpp"
#include "LevelOpenPopup.hpp"
#include "DailyScene2.hpp"
#include "GameSelectScene.hpp"


#include "CustomDirector.h"


#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/LogManager.hpp"
#include "Managers/StrictLogManager.h"
#include "Managers/CacheManager.hpp"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/KitkitVideoPlayer.hpp"
#include "Common/Controls/TouchEventLogger.h"

#include "Games/Video/GameVideoPlayer.hpp"

#include "Utils/TodoUtil.h"

#include "3rdParty/CCNativeAlert.h"


namespace CoopScene2Space {
    const Size coopSize = Size(2560, 1800);
    const Size roomSize = Size(810, 532);
    const float birdScale = 0.7;
    float coopScale;
    
    

    const int glowZ = 2000;
    const int zoomZ = 3000;
    
    

    class Room : public Node
    {
    public:
        CREATE_FUNC(Room);
        
        Sprite *nest, *shadow, *light, *cover;
        
        Bird *bird;
        std::string levelID;
        bool isLightUp;
        
        
        
        void setupRoom(LevelCurriculum &cur);
        void addShadow(bool isEgg = false);
        void turnLight(bool turnOn, bool animate = false);
        
    };

    

};

using namespace CoopScene2Space;

Scene* CoopScene2::createScene(CoopType type)
{
    StrictLogManager::shared()->courseChoice_Begin();

    // 'scene' is an autorelease object
    auto scene = Scene::create();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = CoopScene2::create();
    layer->initWithType(type);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool CoopScene2::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    return true;
}

void CoopScene2::initWithType(CoopType type)
{
    _coopType = type;
    
    SoundEffect::lightEffect().preload();
    
    setTouchEnabled(true);
    
    auto dirSize = Director::getInstance()->getWinSize();
    
    coopScale = MIN(dirSize.width / coopSize.width,
                    dirSize.height / coopSize.height);
    
    
    
    _coopView = Node::create();
    _coopView->setContentSize(coopSize);
    _coopView->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _coopView->setScale(coopScale);
    _coopView->setPosition(dirSize/2.f);
    addChild(_coopView);
    
    
    
    
    setupCoop();
    
    
    _currentZ = 100;
    
    setupDebug();
    
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, dirSize.height-25));
    backButton->onBack = [] {
        StrictLogManager::shared()->courseChoice_End();
    };
    addChild(backButton);
}


void CoopScene2::confirmReset()
{
    
    auto pSize = Size(500, 500);
    auto popup = PopupBase::create(this, pSize);
    
    auto textedit = ui::TextField::create("password to reset", "fonts/mukta-bold.ttf", 50);
    textedit->setPosition(Vec2(pSize.width*0.5, pSize.height*0.7));
    popup->addChild(textedit);
    
    {
        auto btn = ui::Button::create();
        btn->setTitleText("Cancel");
        btn->setTitleFontName("fonts/mukta-bold.ttf");
        btn->setTitleColor(Color3B::WHITE);
        btn->setTitleFontSize(50);
        
        btn->addClickEventListener([popup](Ref*) {
            popup->dismiss(true);
        });
        
        btn->setPosition(Vec2(pSize.width*0.25, pSize.height*0.3));
        popup->addChild(btn);
        
    }
    
    {
        auto btn = ui::Button::create();
        btn->setTitleText("Reset");
        btn->setTitleFontName("fonts/mukta-bold.ttf");
        btn->setTitleColor(Color3B::WHITE);
        btn->setTitleFontSize(50);
        
        btn->addClickEventListener([popup, textedit](Ref*) {
            if (textedit->getString()=="2019") {
                popup->dismiss(false);
                NativeAlert::show("Playdata cleared!", "", "OK");
                UserManager::getInstance()->resetStatus();
                Director::getInstance()->popToRootScene();
                
            } else {
                NativeAlert::show("Wrong Password!", "", "OK");
            }
        });
        
        btn->setPosition(Vec2(pSize.width*0.75, pSize.height*0.3));
        popup->addChild(btn);
    }
    
    

    
    popup->show(this, true);

}



void CoopScene2::setupCoop()
{
    _coopView->removeAllChildren();

    
    auto bg = Sprite::create("CoopScene/coop_bg.jpg");
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    bg->setPosition(Vec2(coopSize.width/2, 0));
    _coopView->addChild(bg);

    
    auto roofName = (this->_coopType== CT_LITERACY) ? "CoopScene/coop_roof_literacy.png" : "CoopScene/coop_roof_math.png";
    auto roof = Sprite::create(roofName);
    roof->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    roof->setPosition(Vec2(coopSize.width/2, bg->getContentSize().height - 270));
    _coopView->addChild(roof);
    
    _rooms.clear();
    
    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
    auto category = (this->_coopType== CT_LITERACY) ? 'L' : 'M';
    
    
    auto panelName = (this->_coopType== CT_LITERACY) ? "CoopScene/coop_woodpanel_title_literacy.png" : "CoopScene/coop_woodpanel_title_math.png";
    auto panel = Sprite::create(panelName);
    panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    panel->setPosition(Vec2(coopSize.width/2.f, coopSize.height - 80));
    
    auto labelText = LanguageManager::getInstance()->getLocalizedString((this->_coopType== CT_LITERACY) ? "English" : "Math");
    
    auto label = TodoUtil::createLabel(labelText, 100, Size::ZERO, "fonts/chanakya.ttf", Color4B(255, 252, 219, 255));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    label->setPosition(panel->getContentSize()/2);
    panel->addChild(label);
    
    _coopView->addChild(panel);
    
    
    
    int maxLevel = -1;
    
    for (auto it : CurriculumManager::getInstance()->levels) {
        LevelCurriculum cur = it.second;
        if (cur.lang!=lang) continue;
        if (cur.category!=category) continue;
    
        bool open = UserManager::getInstance()->isLevelOpen(cur.levelID);
        bool cleared = UserManager::getInstance()->isLevelCleared(cur.levelID);
        
        if (cur.categoryLevel==0 && !open) {
            UserManager::getInstance()->setLevelOpen(cur.levelID);
        }
        
        
        if (cleared) {
            maxLevel = cur.categoryLevel;
        }
    }
    
    
    
    for (auto it : CurriculumManager::getInstance()->levels) {
        LevelCurriculum cur = it.second;
        if (cur.lang!=lang) continue;
        if (cur.category!=category) continue;
        
        Room *room = Room::create();
        room->setupRoom(cur);
        
        float x = (cur.categoryLevel%3) * roomSize.width + (coopSize.width-roomSize.width*3)/2.f;
        float y = (2-(cur.categoryLevel/3)) * roomSize.height + 36;
        
        room->setPosition(x, y);
        _coopView->addChild(room);
        
        _rooms.push_back(room);
        
        auto bird = room->bird;
        
        if (UserManager::getInstance()->isLevelCleared(bird->getLevelID())) {
            room->turnLight(true, false);
        }
        
        bird->onTouchBegan = [bird, this, room](){
            if (!isTouchEnabled()) return;
            if (!room->isLightUp) return;
            bird->runTouchAnimation();
            
            
        };
        
        bird->onTouchEnded = [this, room]() {
            
            
            if (!isTouchEnabled()) return;
            if (!room->isLightUp) return;
            
            setTouchEnabled(false);
            
            if (UserManager::getInstance()->getGuideDayStatus() == guideDayType::touchFirst) {
                UserManager::getInstance()->setGuideDayStatus(guideDayType::recognizeDay);
            }
            
            auto bird = room->bird;
            
            auto c = bird->getCategory();
            auto l = bird->getCategoryLevel();
            auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
            
            auto cur = CurriculumManager::getInstance()->findCurriculum(c, l);
            if (cur) {
                auto levelID = CurriculumManager::getInstance()->makeLevelID(lang, c, l);
                
                if (UserManager::getInstance()->isLevelOpen(levelID)) {
                    
                    StrictLogManager::shared()->courseChoice_TouchAnimal(levelID);
                    this->showDailyScene(levelID);
                    
                    
                } else {
                    if (UserManager::getInstance()->getPretestProgressType(levelID) == PretestProgressType::required) {
                        UserManager::getInstance()->setCurrentDay(levelID, cur->numDays);
                        UserManager::getInstance()->setCurrentLevelID(levelID);
                        UserManager::getInstance()->setPlayingDay(cur->numDays);
                        
                        auto popup = LevelOpenPopup::create(this);
                        popup->setup(levelID);
                        popup->onloadCallback = CallFunc::create([this](){
                            this->setTouchEnabled(true);
                        });

                        popup->show(this, true);
                        
                        popup->onOpenLevel = [this](){
                            auto scene = GameSelectScene::createScene();
                            scene->setName("GameSelectScene");
                            auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(scene));
                            fadeScene->setName("(TransitionFade GameSelectScene)");
                            Director::getInstance()->pushScene(fadeScene);
                        };
                        
                    } else {
                        StrictLogManager::shared()->courseChoice_ShowNewAnimal(levelID);
                        UserManager::getInstance()->setLevelOpen(levelID);
                        this->hatchEgg(room);
                    }
                }
            } else {
                setTouchEnabled(true);
            }
            
            
            
            
        };
        
    }
    
    
    

}


void CoopScene2::setupDebug()
{
    auto dirSize = Director::getInstance()->getWinSize();
    
    const Size debugViewSize = Size(dirSize.width-60, 200);
    _debugView = LayerColor::create(Color4B(0, 0, 0, 64), debugViewSize.width, debugViewSize.height);
    _debugView->setPosition(30, dirSize.height-debugViewSize.height);
    _debugView->setContentSize(dirSize);
    addChild(_debugView, _currentZ++);
    
    
    
    bool isDebug = UserManager::getInstance()->isDebugMode();
    bool isGameTesting = UserManager::getInstance()->isGameTestingMode();
    bool isResetAllowed = UserManager::getInstance()->isResetAllowed();
    
    
    
    
    if (!isDebug) {
        _debugView->setVisible(false);
        
        if (isResetAllowed) {
            ui::Button* resetButton = ui::Button::create();
            resetButton->setTitleText("reset");
            resetButton->setTitleFontSize(50);
            resetButton->setPosition(Vec2(dirSize.width-200, dirSize.height-100));
            resetButton->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    this->confirmReset();
                }
            }  );
            this->addChild(resetButton);
        }
    } else {
        
        Label* versionTitle = Label::createWithTTF(UserManager::getInstance()->getAppVersion(), "fonts/mukta-bold.ttf", 42);
        versionTitle->setTextColor(Color4B::WHITE);
        versionTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        versionTitle->setPosition(Vec2(30, debugViewSize.height/2));
        _debugView->addChild(versionTitle);
        
        
        
        ui::Button* resetButton = ui::Button::create();
        resetButton->setTitleText("reset");
        resetButton->setTitleFontSize(50);
        resetButton->setPosition(Vec2(debugViewSize.width-200, debugViewSize.height/2));
        resetButton->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                this->confirmReset();
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
            auto langStr = LanguageManager::getInstance()->getCurrentLocaleCode();
            
            auto txt = "Lang: " + langStr;
            langButton->setTitleText(txt);
            langButton->setTitleFontSize(50);
            langButton->setPosition(Vec2(debugViewSize.width-1500, debugViewSize.height/2));
            
            langButton->addTouchEventListener([langButton, this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    
                    LanguageManager::LocaleType next = LanguageManager::getInstance()->findNextLocale();
                    LanguageManager::getInstance()->setCurrentLocale(next);
                    CurriculumManager::getInstance()->loadData();
                    auto langStr = LanguageManager::getInstance()->getCurrentLocaleCode();
                    auto txt = "Lang: " + langStr;
                    langButton->setTitleText(txt);
                    
                    this->setupCoop();
                    this->checkLight();
                    
                    
                }
            }  );
            
            
            _debugView->addChild(langButton);
            
        }
        
        
        {
            ui::Button* openButton = ui::Button::create();
            
            openButton->setTitleText("OpenAll");
            openButton->setTitleFontSize(50);
            openButton->setPosition(Vec2(debugViewSize.width-1800, debugViewSize.height/2));
            
            openButton->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
                    auto category = (this->_coopType== CT_LITERACY) ? 'L' : 'M';
                    
                    for (auto it : CurriculumManager::getInstance()->levels) {
                        LevelCurriculum cur = it.second;
                        if (cur.lang!=lang) continue;
                        if (cur.category!=category) continue;
                        
                        UserManager::getInstance()->setLevelOpen(cur.levelID);
                    
                    }
                    
                    this->setupCoop();
                    this->checkLight();
                    
                    
                }
            }  );
            
            
            _debugView->addChild(openButton);
            
        }
        {
            ui::Button* clearButton = ui::Button::create();
            
            clearButton->setTitleText("ClearAll");
            clearButton->setTitleFontSize(50);
            clearButton->setPosition(Vec2(debugViewSize.width-2000, debugViewSize.height/2));
            
            clearButton->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
                if (e == ui::Widget::TouchEventType::ENDED) {
                    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
                    auto category = (this->_coopType== CT_LITERACY) ? 'L' : 'M';
                    
                    for (auto it : CurriculumManager::getInstance()->levels) {
                        LevelCurriculum cur = it.second;
                        if (cur.lang!=lang) continue;
                        if (cur.category!=category) continue;
                        
                        UserManager::getInstance()->setLevelOpen(cur.levelID);
                        for (int i=1; i<=cur.days.size(); i++) {
                            UserManager::getInstance()->setDayCleared(cur.levelID, i);
                        }
                    }
                    
                    this->setupCoop();
                    this->checkLight();
                    
                    
                }
            }  );
            
            
            _debugView->addChild(clearButton);
            
        }
        
        
    }
}


void CoopScene2::onEnter()
{
    Layer::onEnter();
    
    GameSoundManager::getInstance()->stopBGM();
    
    CacheManager::getInstance()->loadDailyCache();
    
    auto levelID = UserManager::getInstance()->getCurrentLevelID();
    
    for (auto room : _rooms) {
        auto bird = room->bird;
        if (bird->getLevelID() == levelID && bird->getStatus() == Bird::BirdStatus::EGG_HATCHED) {
            bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(room->levelID));
            bird->loadAnimation();
            bird->runTouchAnimation();
        }
    }

    if (this->_coopType== CT_LITERACY && UserManager::getInstance()->getGuideDayStatus() == guideDayType::recognizeDay) {
        zoomIn(1, 0);
        this->runAction(Sequence::create(DelayTime::create(1.f),
            CallFunc::create([this](){
                zoomIn(0, 0.4);
            }),
        nullptr));
        UserManager::getInstance()->setGuideDayStatus(guideDayType::finish);
    }

}


void CoopScene2::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    GameSoundManager::getInstance()->playBGM("Common/Music/SFX_AllAnimals_IncAmbience.m4a");
    setTouchEnabled(true);
    
    auto levelID = UserManager::getInstance()->getCurrentLevelID();
    
    for (auto room : _rooms) {
        auto bird = room->bird;
        //if (bird->getStatus() != Bird::BirdStatus::EGG) continue;
        switch(UserManager::getInstance()->getPretestProgressType(bird->getLevelID())) {
            case PretestProgressType::pass: {
                auto cur = CurriculumManager::getInstance()->findCurriculum(bird->getLevelID());
                for (int i=0; i<cur->numDays-1; i++) {
                    UserManager::getInstance()->setDayCleared(levelID, i+1);
                }
            }
            case PretestProgressType::fail:
                _holdCheckLight = true;
                UserManager::getInstance()->setPretestProgressType(bird->getLevelID(), PretestProgressType::finish);
                UserManager::getInstance()->setLevelOpen(bird->getLevelID());
                hatchEgg(room);
                break;
            default: break;
        }
    }
    
    if (!_holdCheckLight) checkLight();
    
    if (this->_coopType== CT_LITERACY && UserManager::getInstance()->getGuideDayStatus()==guideDayType::touchFirst) {
        this->runAction(Sequence::create(DelayTime::create(1.f),
            CallFunc::create([this](){
                zoomIn(1, 0.4);
            }),
        nullptr));
    }

    
}

void CoopScene2::zoomIn(int level, float duration)
{
    if (level==_zoomLevel) return;
    _zoomLevel = level;
    
    float targetScale;
    Vec2 targetPos;
    
    auto dirSize = Director::getInstance()->getWinSize();
    
    
    if (level==1) {
        targetScale = 1.5*coopScale;
        targetPos = dirSize/2.0 + Size(630, -dirSize.height/2+350);
    } else {
        targetScale = 1.0*coopScale;
        targetPos = dirSize/2.0;
    }
    
    auto coopSpawn = Spawn::create(MoveTo::create(duration, targetPos), ScaleTo::create(duration, targetScale), NULL);
    _coopView->runAction(coopSpawn);
    
}




void CoopScene2::hatchEgg(CoopScene2Space::Room *room)
{
    
    auto bird = room->bird;
    auto birdPosInRoom = bird->getPosition();
    auto birdPosInCoop = this->convertToNodeSpace(room->convertToWorldSpace(bird->getPosition()));
    
    float birdScaleInCoop = 1.0;
    Node *p = bird;
    while (p!=nullptr) {
        birdScaleInCoop *= p->getScale();
        p = p->getParent();
    }
    
    
    //bird->getParent()->reorderChild(bird, bird->getLocalZOrder());
    
    bird->retain();
    bird->removeFromParent();
    bird->setPosition(birdPosInCoop);
    bird->setScale(birdScaleInCoop);
    this->addChild(bird);
    bird->release();
    
    auto viewSize = this->getContentSize();
    
    
    
    setTouchEnabled(false);
    
    float zoomTime = 1.0;
    const float zoomRate = 3.0;
    
    
    room->shadow->removeFromParent();
    
    
    auto orgPos = bird->getPosition();
    auto zoomPos = Vec2(viewSize.width/2, viewSize.height/2-350);
    auto spawn1 = Spawn::create(EaseIn::create(MoveTo::create(zoomTime, zoomPos), 3.0), EaseIn::create(ScaleTo::create(zoomTime, birdScale*zoomRate), 3.0), nullptr);
    auto spawn2 = Spawn::create(EaseIn::create(MoveTo::create(zoomTime, orgPos), 3.0), EaseIn::create(ScaleTo::create(zoomTime, birdScale), 3.0), nullptr);
    
    auto glow = Node::create();
    glow->setContentSize(coopSize);
    glow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    Vec2 glowWorldPos = viewSize/2;
    Vec2 glowLocalPos = _coopView->convertToNodeSpace(glowWorldPos);
    
    glow->setPosition(glowLocalPos);
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
    sparkle->setName("sparkle");
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
        
        
        
	}), nullptr);
	auto seq2 = Sequence::create(DelayTime::create(2.0),
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
        CallFunc::create([this, room, bird, birdPosInRoom, s1, s2, s3, s4](){
        SoundEffect::particleEffect().play();
        
        //bird->getParent()->reorderChild(bird, bird->getLocalZOrder());
        bird->retain();
        bird->removeFromParent();
        bird->setPosition(birdPosInRoom);
        room->addChild(bird);
        bird->release();
        
        const auto blinkTime = 0.3;
        
        s1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
        
        s2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
        
        
        s3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(random(0.0, blinkTime)), ScaleTo::create(blinkTime, 1.0), ScaleTo::create(blinkTime, 0.0), nullptr)));
        
        s4->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(blinkTime*2.3, 0.9), ScaleTo::create(blinkTime*2.3, 1.1), nullptr)));

        
        
        room->addShadow();
        setTouchEnabled(true);
        
    }),
    nullptr);
    
    bird->runAction(Sequence::create(seq, seq2, nullptr));
    
    runAction(Sequence::create(
        DelayTime::create(9.f),
        CallFunc::create([this, bird](){
        bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
        bird->loadAnimation();

            if (_holdCheckLight) {
                checkLight();
                _holdCheckLight = false;
            }
        }),
        nullptr
    ));

}

void CoopScene2::showDailyScene(std::string levelID)
{
    UserManager::getInstance()->setCurrentLevelID(levelID);
    
    if (UserManager::getInstance()->isGameTestingMode()) {
        auto scene = GameSelectScene::createScene();
        scene->setName("GameSelectScene");
        auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(scene));
        fadeScene->setName("(TransitionFade GameSelectScene)");
        Director::getInstance()->pushScene(fadeScene);
        
        setTouchEnabled(true);
        //((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
        return;
    }
    
    
    auto dailyScene = DailyScene2::createScene(levelID);
    dailyScene->setName("DailyScene2");
    auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(dailyScene));
    fadeScene->setName("(TransitionFade DailyScene2)");
    Director::getInstance()->pushScene(fadeScene);
    
//    popup->show();
//    
//    popup->onSelectDay = [this, levelID](int day) {
//        UserManager::getInstance()->setCurrentLevelID(levelID);
//        UserManager::getInstance()->setCurrentDay(day);
//        
//        
//        //((CustomDirector*)Director::getInstance())->popSceneWithTransition<TransitionFade>(0.5);
//        auto scene = GameSelectScene::createScene();
//        
//        Director::getInstance()->pushScene(TransitionFade::create(0.8, scene));
//        
//    };
//    
//    popup->onDismiss = [this]() {
//        setTouchEnabled(true);
//        GameSoundManager::getInstance()->playBGM("Common/Music/SFX_AllAnimals_IncAmbience.m4a");
//        
//    };
    
    
}

void CoopScene2::checkLight()
{
    
    
    int maxCleared = -1;
    int maxOpened = -1;
    
    for (auto room : _rooms) {
        auto bird = room->bird;
        auto level = bird->getCategoryLevel();
        if (UserManager::getInstance()->isLevelCleared(bird->getLevelID())) {
            maxCleared = MAX(maxCleared, level);
        }
        if (UserManager::getInstance()->isLevelOpen(bird->getLevelID())) {
            maxOpened = MAX(maxOpened, level);
        }
    }

    int light = maxCleared;
    
    /*
    if (maxCleared<2) {
        light = 2;
    } else {
        light = maxCleared+1;
    }*/
    light = maxCleared+1;
    light = MAX(light, maxOpened);
    
    
    bool newTurnOn = false;
    
    for (auto room : _rooms) {
        auto bird = room->bird;
        auto level = bird->getCategoryLevel();
        
        bool toTurnOn = false;
        if (level<=light && !room->isLightUp) toTurnOn = true;
        
        if (toTurnOn) {
            if (!room->isLightUp) newTurnOn = true;
            room->turnLight(true, true);
        }
    }
    
    if (newTurnOn) SoundEffect::lightEffect().play();
    

    
}


///////////

void Room::setupRoom(LevelCurriculum &cur)
{
    cover = nullptr;
    light = nullptr;
    isLightUp = false;
    
    bool isOpen = UserManager::getInstance()->isLevelOpen(cur.levelID);
    
    this->levelID = cur.levelID;
    
    
    this->setContentSize(roomSize);
    
    if (true /*cur.categoryLevel>0*/) {
        
        string panelFilename = cur.category=='L' ? "coop_woodpanel_level_literacy.png" : "coop_woodpanel_level_math.png";
        //if (cur.categoryLevel==0) panelFilename = "coop_woodpanel_prek.png";
        auto panel = Sprite::create("CoopScene/"+panelFilename);
        
        Vec2 panelPos = Vec2(panel->getContentSize().width/2.f+30, 510);
        
        panel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        panel->setPosition(panelPos);
        this->addChild(panel);
        
        auto panelLabel = TodoUtil::createLabel(TodoUtil::itos(cur.categoryLevel), 70, Size::ZERO, "fonts/mukta-bold.ttf", Color4B(255, 252, 219, 255));
        panelLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        panelLabel->setPosition(panel->getContentSize()/2+Size(0, -5));
        panel->addChild(panelLabel);
        
        if (cur.categoryLevel == 0) panel->setVisible(false);
    }
    
    {
        string nestName = cur.category=='L' ? "coop_slot_bottom_literacy.png" : "coop_slot_bottom_math.png";
        auto nest = Sprite::create("CoopScene/"+nestName);
        nest->setPosition(Vec2(roomSize.width/2, 114));
        this->addChild(nest);
        
    }
    
    bird = Bird::create(cur.category, cur.categoryLevel, cur.levelID);
    
//    int x = cur.category=='L' ? 1-(cur.categoryLevel%2) : 2+(cur.categoryLevel%2);
//    int y = 2-(cur.categoryLevel/2);
//    Point p = Point(x, y);
    
    Vec2 birdPos = Vec2(roomSize.width/2, 100);
    bird->setPosition(birdPos);
    if (isOpen) {
        bird->setScale(birdScale);
        bird->setStatus(Bird::BirdStatus::EGG_HATCHED);
        bird->setBirdProgress(UserManager::getInstance()->ratioDayCleared(cur.levelID));
        bird->loadAnimation();
    } else {
        bird->setStatus(Bird::BirdStatus::EGG);
    }
    
//    
//    string nestName = (cur.category=='M') ? "coop_math_nest.png" : "coop_english_nest.png";
//    auto nest = Sprite::create("CoopScene/"+nestName);
//    nest->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//    nest->setPosition(birdPos + Vec2(0, 20));
//    this->addChild(nest);
    
    
    addShadow(!isOpen);
    
    
    
    
    this->addChild(bird);

    
    
    
    
    light = Sprite::create("CoopScene/coop_cell_light.png");
    light->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    light->setPosition(roomSize.width/2, roomSize.height-22);
    this->addChild(light);

    cover = Sprite::create("CoopScene/coop_cell_shade.png");
    //cover->setOpacity(235);
    cover->setPosition(roomSize.width/2, roomSize.height/2);
    this->addChild(cover);

    
    
    turnLight(false);
}

void Room::addShadow(bool isEgg)
{
    if (!isEgg) {
        shadow = Sprite::create("CoopScene/coop_bird_shadow.png");
        shadow->setScale(bird->getBoundingBox().size.width / shadow->getContentSize().width);
    } else {
        shadow = Sprite::create("CoopScene/coop_egg_shadow.png");
    }
    
    shadow->setPosition(bird->getPosition());
    this->addChild(shadow);

}

void Room::turnLight(bool turnOn, bool animate)
{
    if (isLightUp == turnOn) return;
    
    isLightUp = turnOn;
    
    auto lightOn = [this](bool on) {
        light->setVisible(on);
        cover->setVisible(!on);
    };
    
    
    if (!animate) {
        lightOn(turnOn);
    } else {
        
        auto seq = Sequence::create(DelayTime::create(random(0.1, 0.3)),
                                    CallFunc::create([lightOn, turnOn](){ lightOn(turnOn); }),
                                    DelayTime::create(random(0.02, 0.10)),
                                    CallFunc::create([lightOn, turnOn](){ lightOn(!turnOn); }),
                                    DelayTime::create(random(0.02, 0.10)),
                                    CallFunc::create([lightOn, turnOn](){ lightOn(turnOn); }),
                                    DelayTime::create(random(0.02, 0.10)),
                                    CallFunc::create([lightOn, turnOn](){ lightOn(!turnOn); }),
                                    DelayTime::create(random(0.20, 0.30)),
                                    CallFunc::create([lightOn, turnOn](){
            
                                        lightOn(turnOn);
                                    }),
                                    nullptr);
        
        this->runAction(seq);
        
        
        
        
    }
    

    
    
}
