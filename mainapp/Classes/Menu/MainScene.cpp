//
//  MainScene.cpp
//  KitkitSchool
//
//  Created by Sungwoo Kang on 6/20/16.
//
//

#include <functional>

#include "CCAppController.hpp"
#include "Utils/TodoUtil.h"


#include "MainScene.hpp"
#include "CoopScene.hpp"
#include "CoopScene2.hpp"


#include "GradeSelector.hpp"

#include "Bird.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/PopupBase.hpp"
#include "Common/Controls/TouchEventLogger.h"
#include "Common/Controls/CoinTab.hpp"

#include "Managers/LanguageManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"
#include "Managers/CacheManager.hpp"

#include "Common/Effects/FireworksEffect.hpp"

#include "3rdParty/CCNativeAlert.h"
#include "CustomDirector.h"

namespace MainSceneSpace {
    const Size designSize = Size(2560,1800);
    static Size visibleSize;
    static Size frameSize;
    
    const Vec2 coop1Pos = Vec2(designSize.width-220-241, designSize.height-660-172);
    const Vec2 coop2Pos = Vec2(designSize.width-220-241-550, designSize.height-660-172);
    
    //static Vec2 quitPos;
    
    const Size quitNodeSize = Size(223, 158);
    
  //  const int coopBackZ = -100;

    
    const string fontName = "fonts/chanakya.ttf";
    
    bool isDemo = true;
};

using namespace MainSceneSpace;


static bool __firstEnter = true;

Scene* MainScene::createScene()
{
    
    CCLOG("MainScene::createScene");
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    scene->setContentSize(visibleSize);
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    layer->setTag(MENU_SCENE_ID);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    
    // return the scene
    return scene;
}




bool MainScene::init()
{
    
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    manageCache();
    
    //std::string s = Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    //CCLOG("Cached texture info : %s",s.c_str());
    
    visibleSize = Director::getInstance()->getVisibleSize();
    _debugView = nullptr;
    
    _rootNode = Node::create();
    _rootNode->setContentSize(designSize);
    _rootNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _rootNode->setPosition(Vec2(visibleSize.width/2, 0));
    
    addChild(_rootNode);
    
    auto skySize = Size(designSize.width, designSize.height*2);
    _skyNode = Node::create();
    _skyNode->setContentSize(skySize);
    _rootNode->addChild(_skyNode);
    
    _groundNode = Node::create();
    _groundNode->setContentSize(designSize);
    _rootNode->addChild(_groundNode);
    
    Sprite* sky = Sprite::create("MainScene/main_bg_sky.png");
    sky->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sky->setPosition(0,0);
    auto skySpriteSize = sky->getContentSize();
    auto skyScale = MAX(skySize.width/skySpriteSize.width, skySize.height/skySpriteSize.height);
    sky->setScale(skyScale);
    _skyNode->addChild(sky);
    
    
    
    Sprite* cloud1 = Sprite::create("MainScene/cloud_day_1.png");
    cloud1->setPosition(Vec2(visibleSize.width, random(1350, 1450)));
    cloud1->runAction(RepeatForever::create(Sequence::create(
                                                             MoveBy::create(random(40.0, 50.0), Vec2(visibleSize.width+1000, 0)),
                                                             MoveTo::create(0, Vec2(-500, cloud1->getPositionY())),
                                                             nullptr)));
    _skyNode->addChild(cloud1);
    
    Sprite* cloud2 = Sprite::create("MainScene/cloud_day_2.png");
    cloud2->setPosition(Vec2(visibleSize.width, random(1450, 1550)));
    cloud2->runAction(RepeatForever::create(Sequence::create(
                                                             
                                                             MoveBy::create(random(35.0, 40.0), Vec2(visibleSize.width+1000, 0)),
                                                             MoveTo::create(0, Vec2(-500, cloud2->getPositionY())),
                                                             nullptr)));
    _skyNode->addChild(cloud2);

    Sprite* cloud3 = Sprite::create("MainScene/cloud_day_3.png");
    cloud3->setPosition(Vec2(visibleSize.width, random(1650, 1750)));
    cloud3->runAction(RepeatForever::create(Sequence::create(
                                                             MoveBy::create(random(25.0, 35.0), Vec2(visibleSize.width+1000, 0)),
                                                             MoveTo::create(0, Vec2(-500, cloud3->getPositionY())),
                                                             nullptr)));
    _skyNode->addChild(cloud3);


    /*
    {
        ui::Button* test = ui::Button::create("icons/game_icon_frame_completed.png");
        
        test->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        test->setPosition(Vec2(visibleSize.width*0.3, visibleSize.height-300));
        test->addClickEventListener([this](Ref*) {
            
            auto effect = FireworksEffect::create();
            effect->setPosition(Vec2(1024, 1000));
            this->addChild(effect);
            effect->start();
            
        });
        addChild(test);
    }
    
    {
        ui::Button* test = ui::Button::create("icons/game_icon_frame_completed.png");
        
        test->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        test->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height-300));
        test->addClickEventListener([this](Ref*) {
            auto cl = LayerColor::create(Color4B(255, 0, 0, 64));
            Director::getInstance()->getRunningScene()->addChild(cl);
            auto shake = Sequence::create(EaseOut::create(MoveBy::create(0.06, Vec2(50, 0)), 2.0),
                                          EaseInOut::create(MoveBy::create(0.12, Vec2(-100, 0)), 2.0),
                                          EaseOut::create(MoveBy::create(0.06, Vec2(50, 0)), 2.0),
                                          CallFunc::create([cl](){
                cl->removeFromParent();
            }),
                                          nullptr);
            Director::getInstance()->getRunningScene()->runAction(shake);
            
        });
        addChild(test);
    }
    
    */
    
    
    Sprite* mountain = Sprite::create("MainScene/right_mountain.png");
    mountain->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    mountain->setPosition(designSize.width, 730);
    _groundNode->addChild(mountain);
    
    
    _debugCommand = "";
    
    ui::Button* coop1 = ui::Button::create("MainScene/main_coop_math.png");
    coop1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    coop1->setPosition(coop1Pos);
    coop1->addClickEventListener([this](Ref*) {
        if (((CustomDirector*)Director::getInstance())->isNextScene()) return;
        
        if (_transitionBegins) {
            return;
        }
        
        if (_debugCommand=="lrllrlrr") {
            this->_debugCoopNo=1;
            this->confirmDebug();
            return;
        }
        
        _transitionBegins = true;
        StrictLogManager::shared()->curriculumChoice_TouchCoop();
        zoomCoop(_coop1Node);
    });
    _groundNode->addChild(coop1);
    _coop1Node = coop1;
    
    ui::Button* coop2 = ui::Button::create("MainScene/main_coop_literacy.png");
    coop2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    coop2->setPosition(coop2Pos);
    coop2->addClickEventListener([this](Ref*) {
        if (((CustomDirector*)Director::getInstance())->isNextScene()) return;
        
        if (_transitionBegins) {
            return;
        }
        
        if (_debugCommand=="lrllrlrr") {
            this->_debugCoopNo=2;
            this->confirmDebug();
            return;
        }
        if (UserManager::getInstance()->getGuideCoopStatus() != guideCoopType::finish) UserManager::getInstance()->setGuideCoopStatus(guideCoopType::recognizeCoop);
        
        _transitionBegins = true;
        StrictLogManager::shared()->curriculumChoice_TouchCoop();
        zoomCoop(_coop2Node);
    });
    _groundNode->addChild(coop2);
    _coop2Node = coop2;
    
    
    
    Sprite* grass = Sprite::create("MainScene/day_grass_ground.png");
    grass->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    grass->setPosition(Vec2(0,0));
    _groundNode->addChild(grass);

    _rootScale = 1.f;
    if (visibleSize.width > designSize.width) {
        _rootScale = (visibleSize.width/designSize.width);
        _rootNode->setScale(_rootScale);
        
        frameSize = visibleSize/_rootScale;
        
        
    } else {
        frameSize = visibleSize;
    }
    
    _frameNode = Node::create();
    _frameNode->setContentSize(frameSize);
    _frameNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frameNode->setPosition(Vec2(designSize.width/2, 0));
    _rootNode->addChild(_frameNode);
    
    
    //_leavesLeft = Sprite::create("MainScene/main_leaves_left.png");
    {
        auto btn = cocos2d::ui::Button::create("MainScene/main_leaves_left.png");
        btn->setZoomScale(0);
        btn->addClickEventListener([this](Ref*) {
            this->_debugCommand = this->_debugCommand + "l";
        });
        _leavesLeft = btn;
        
    }
    _leavesLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _leavesLeft->setPosition(Vec2(0, frameSize.height-474));
    _frameNode->addChild(_leavesLeft);
    
    
    //_leavesRight = Sprite::create("MainScene/main_leaves_right.png");
    {
        auto btn = cocos2d::ui::Button::create("MainScene/main_leaves_right.png");
        btn->setZoomScale(0);
        btn->addClickEventListener([this](Ref*) {
            this->_debugCommand = this->_debugCommand + "r";
        });
        _leavesRight = btn;
        
    }
    
    _leavesRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _leavesRight->setPosition(Vec2(frameSize.width, frameSize.height-382));
    _frameNode->addChild(_leavesRight);
    
    auto coopWorldPos = _groundNode->convertToWorldSpace(coop2->getPosition());
    auto coopRootPos = _rootNode->convertToNodeSpace(coopWorldPos);
    
    auto highlight = Sprite::create("MainScene/coop_highlight_02.png");
    highlight->setAnchorPoint(Vec2(0.6, 0.54));
    highlight->setPosition(coopRootPos);
    highlight->setOpacity(0);
    highlight->setScale(1.1);
    _rootNode->addChild(highlight);
    _highlight = highlight;
 
    {
        //quitPos = Vec2(frameSize.width - 30, frameSize.height-30);
        _quitButton = ui::Button::create("MainScene/mainscreen_exitbutton_normal.png", "MainScene/mainscreen_exitbutton_active.png");
        _quitButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _quitButton->setPosition(Vec2(540, designSize.height-660-190));
        _groundNode->addChild(_quitButton);
        
        //_quitButton->setPosition(quitPos);
        //_frameNode->addChild(_quitButton);
    
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
                if (((CustomDirector*)Director::getInstance())->isNextScene()) return;
                UserManager::getInstance()->sendAppToBack();
                
            }
        };
        _quitButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, _quitButton);
        _quitButton->addClickEventListener([this](Ref*){
            if (((CustomDirector*)Director::getInstance())->isNextScene()) return;
            StrictLogManager::shared()->curriculumChoice_End();
            UserManager::getInstance()->sendAppToBack();
        });

    }
    
    
    
    
    SoundEffect::coopClickEffect().preload();
    

//    if (isDemo) {
    {
        
        _coinTab = CoinTab::create();
        _coinTab->setPosition(visibleSize - Size(50, 50));
        _coinTab->setVisible(false);
        addChild(_coinTab);
        {
            _reviewModeLabel = TodoUtil::createLabel("You are in admin mode: please use the features below to access all of Kitkit School’s content. Select “Add Coins” to access the tools section.", 50, Size(1300, 300), "fonts/mukta-bold.ttf", Color4B(0, 0, 0, 255));
            _reviewModeLabel->setPosition(Vec2(visibleSize.width/2, visibleSize.height-150));
            this->addChild(_reviewModeLabel);
            
            _signLanguageModeLabel = TodoUtil::createLabel("Sign Language Mode is on", 50, Size::ZERO, "fonts/mukta-bold.ttf", Color4B(116, 198, 225, 255));
            _signLanguageModeLabel->setPosition(Vec2(visibleSize.width/2, 150));
            this->addChild(_signLanguageModeLabel);
        }
        
        
        {
            _resetBtn = ui::Button::create("MainScene/for-manager_button_normal.png","MainScene/for-manager_button_touch.png");
            auto l = TodoUtil::createLabel("ÚèâðÅ", 50, Size::ZERO, "fonts/chanakya.ttf", Color4B(0, 0, 0, 255)); // रीसेट
            l->setPosition(_resetBtn->getContentSize()/2 + Size(0, 10));
            _resetBtn->addChild(l);

            _resetBtn->setPosition(Vec2(visibleSize.width/2-400, visibleSize.height-400));
            _resetBtn->addTouchEventListener([this, l](Ref*,ui::Widget::TouchEventType e) {
                if (_resetBtn->isHighlighted()) {
                    l->setPosition(_resetBtn->getContentSize()/2 + Size(0, 5));
                } else {
                    l->setPosition(_resetBtn->getContentSize()/2 + Size(0, 10));
                }
                if (e == ui::Widget::TouchEventType::ENDED) {
                    UserManager::getInstance()->resetStatus();
                    UserManager::getInstance()->updateStars(0);
                    _coinTab->updateCoinLabel();
                    NativeAlert::show("User Progress Reset!", "", "OK");
                    __firstEnter = true;
                    std::function<Scene*(void)> creator = []() {
                        auto scene = MainScene::createScene();
                        scene->setName("MainScene");
                        return scene;
                    };
                    Director::getInstance()->replaceScene(TransitionFade::create(1.f, TodoLoadingScene::createScene(creator)));
                    
                }
                
            }  );
            this->addChild(_resetBtn);
        }
        
        {
            _addCoinsBtn = ui::Button::create("MainScene/for-manager_button_normal.png","MainScene/for-manager_button_touch.png");
            auto l = TodoUtil::createLabel("çâPðUUU ÁôÇô", 50, Size::ZERO, "fonts/chanakya.ttf", Color4B(0, 0, 0, 255)); // सिक्के जोडो
            l->setPosition(_addCoinsBtn->getContentSize()/2 + Size(0, 10));
            _addCoinsBtn->addChild(l);
            
            _addCoinsBtn->setPosition(Vec2(visibleSize.width/2, visibleSize.height-400));
            _addCoinsBtn->addTouchEventListener([this, l](Ref*,ui::Widget::TouchEventType e) {
                if (_addCoinsBtn->isHighlighted()) {
                    l->setPosition(_addCoinsBtn->getContentSize()/2 + Size(0, 5));
                } else {
                    l->setPosition(_addCoinsBtn->getContentSize()/2 + Size(0, 10));
                }
                if (e == ui::Widget::TouchEventType::ENDED) {
                    _coinTab->updateCoinLabel();
                    
                    auto coin = UserManager::getInstance()->getStars();
                    UserManager::getInstance()->updateStars(coin+100);
                    
                    _coinTab->setVisible(true);
                    _coinTab->addCoin(100, _addCoinsBtn->getPosition(), true);
                }
                
            }  );
            this->addChild(_addCoinsBtn);
        }
        
        {
            _openAllBtn = ui::Button::create("MainScene/for-manager_button_normal.png","MainScene/for-manager_button_touch.png");
            auto l = TodoUtil::createLabel("âÕ ¹ôÜ Îô", 50, Size::ZERO, "fonts/chanakya.ttf", Color4B(0, 0, 0, 255)); // सब खोल दो
            l->setPosition(_openAllBtn->getContentSize()/2 + Size(0, 10));
            _openAllBtn->addChild(l);
            _openAllBtn->setPosition(Vec2(visibleSize.width/2+400, visibleSize.height-400));
            _openAllBtn->addTouchEventListener([this, l](Ref*,ui::Widget::TouchEventType e) {
                
                if (_openAllBtn->isHighlighted()) {
                    l->setPosition(_openAllBtn->getContentSize()/2 + Size(0, 5));
                } else {
                    l->setPosition(_openAllBtn->getContentSize()/2 + Size(0, 10));
                }
                if (e == ui::Widget::TouchEventType::ENDED) {
                    auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();
                    
                    for (auto it : CurriculumManager::getInstance()->levels) {
                        LevelCurriculum cur = it.second;
                        
                        /*
                        if (cur.lang!=lang) continue;
                        auto openLevel = 3;
                        if (cur.category=='M') {
                            if (cur.categoryLevel<=0) openLevel = 3;
                            else if (cur.categoryLevel<=2) openLevel = 9;
                            else openLevel = 10;
                        } else {
                            if (cur.categoryLevel<=0) openLevel = 3;
                            else if (cur.categoryLevel<=1) openLevel = 5;
                            else if (cur.categoryLevel<=2) openLevel = 13;
                            else openLevel = 17;
                        }
                        
                        UserManager::getInstance()->setLevelOpen(cur.levelID);
                        openLevel = MIN(openLevel, cur.days.size());
                        for (int i=1; i<=openLevel; i++) {
                            UserManager::getInstance()->setDayCleared(cur.levelID, i);
                        }
                        */
                        
                        UserManager::getInstance()->setLevelOpen(cur.levelID);
                        UserManager::getInstance()->setPretestProgressType(cur.levelID, PretestProgressType::finish);
                        for (int i=0; i<cur.numDays; i++) {
                            UserManager::getInstance()->setDayCleared(cur.levelID, i);
                        }
                    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "setUnlockFishBowl", true);
#endif
                    NativeAlert::show("Cirriculum unlocked!", "", "OK");
                    __firstEnter = true;

                    UserManager::getInstance()->setGuideCoopStatus(guideCoopType::finish);
                    UserManager::getInstance()->setGuideDayStatus(guideDayType::finish);
                    std::function<Scene*(void)> creator = []() {
                        auto scene = MainScene::createScene();
                        scene->setName("MainScene");
                        return scene;
                    };
                    Director::getInstance()->replaceScene(TransitionFade::create(1.f, TodoLoadingScene::createScene(creator)));
                }
            }  );
            this->addChild(_openAllBtn);
        }
        
    }
    
    return true;
}

void MainScene::manageCache() {
    for (auto it : CurriculumManager::getInstance()->levels) {
        LevelCurriculum cur = it.second;
        if (!UserManager::getInstance()->isLevelOpen(cur.levelID)) continue;
        auto bird = Bird::create(cur.category, cur.categoryLevel, cur.levelID);
        bird->loadAnimation();
    }
}

void MainScene::onEnter()
{
    Layer::onEnter();
    visibleSize = Director::getInstance()->getVisibleSize();

    _debugCommand = "";
    
    stopAllActions();
    _coop1Node->stopAllActions();
    _coop1Node->runAction(RotateTo::create(0, 0));
    _coop2Node->stopAllActions();
    _coop2Node->runAction(RotateTo::create(0, 0));
    setScale(1.0);
    setPosition(Vec2::ZERO);
    
    //_rootNode->stopAllActions();
    //_rootNode->setScale(_rootScale);
    //_rootNode->setPosition(Vec2(visibleSize.width/2, 0));
    


    _transitionBegins = false;
    
    _coop1Node->setPosition(coop1Pos);
    _coop1Node->setScale(1);
    
    _coop2Node->setPosition(coop2Pos);
    _coop2Node->setScale(1);
    
   // _groundNode->reorderChild(_coopNode, coopBackZ);
    
    if (__firstEnter)    {
        StrictLogManager::shared()->curriculumChoice_Begin();
        
        __firstEnter = false;
        _transitionBegins = true;
        
        SoundEffect::iconSpawnEffect().preload();
        SoundEffect::coopClickEffect().preload();
        
        const float appearDelay = 2.2;
        
        //auto eng = LanguageManager::getInstance()->isEnglish();
        auto logoPath = LanguageManager::getInstance()->findLocalizedResource("System/logo_phase03.png");
        auto logo = Sprite::create(logoPath);
        logo->setPosition(designSize/2);
        _rootNode->addChild(logo);
        logo->runAction(Sequence::create(DelayTime::create(1.7),
                                         EaseIn::create(MoveBy::create(0.2, Vec2(0, -100)), 2.0),
                                         EaseOut::create(MoveBy::create(1.0, Vec2(0, designSize.height)), 2.0),
                                         CallFunc::create([](){ SoundEffect::iconSpawnEffect().play(); }),
                                         DelayTime::create(0.7),
                                         CallFunc::create([this](){
            //this->scheduleOnce([this](float){ zoomCoop(); }, 0.01, "zoomCoop");
            _transitionBegins = false;
        }),
                                         nullptr));
        
        
        _skyNode->setPosition(Vec2(0, -designSize.height*1.0));
        _skyNode->runAction(Sequence::create(DelayTime::create(0.2), MoveTo::create(1.8, Vec2(0, 0)), nullptr));
        
        _groundNode->setPosition(Vec2(0, -designSize.height));
        _groundNode->runAction(Sequence::create(DelayTime::create(1.0),
                                                EaseIn::create( MoveTo::create(1.0, Vec2(0, 0)), 2.0),
                                                nullptr));
        
        _coop2Node->runAction(Sequence::create(DelayTime::create(2.2),
                                               EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
                                               EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
                                               nullptr));
        
        _coop2Node->runAction(Sequence::create(DelayTime::create(2.0),
            EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
            EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
            CallFunc::create([this](){
                if (UserManager::getInstance()->getGuideCoopStatus() == guideCoopType::visitFirst) {
                    _highlight->runAction(FadeTo::create(1.f,255*0.8));
                    shakeCoop(_coop2Node);
                    setDisableTouchEventForCoopGuide(true);
                }
            }),
        nullptr));
        
        _quitButton->runAction(Sequence::create(DelayTime::create(1.94),
                                                EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
                                                EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
                                                nullptr));
        
        _leavesLeft->setPosition(Vec2(-_leavesLeft->getContentSize().width, frameSize.height-474));
        _leavesLeft->runAction(Sequence::create(
                                                DelayTime::create(2.5),
                                                EaseOut::create(MoveTo::create(0.2, Vec2(0, frameSize.height-474)), 2.0),
                                                nullptr));
        
        _leavesRight->setPosition(Vec2(designSize.width+_leavesLeft->getContentSize().width, frameSize.height-382));
        _leavesRight->runAction(Sequence::create(
                                                 DelayTime::create(2.5),
                                                 EaseOut::create(MoveTo::create(0.2, Vec2(frameSize.width, frameSize.height-382)), 2.0),
                                                 CallFunc::create([this](){
            CacheManager::getInstance()->loadCoopCache();
        }),
                                                 nullptr));
        
        
        
//        _quitButton->setPosition(Vec2(designSize.width+quitNodeSize.width, quitPos.y));
//        _quitButton->runAction(Sequence::create(
//                                               DelayTime::create(2.5),
//                                               EaseOut::create(MoveTo::create(0.2, quitPos), 2.0),
//                                                
//                                               nullptr));
        
        GameSoundManager::getInstance()->playEffectSoundForAutoStart("Common/Sounds/Effect/LogoFall.m4a");

    } else {
        
        if (UserManager::getInstance()->getGuideCoopStatus() == guideCoopType::recognizeCoop) {
            
            runAction(Sequence::create(
                DelayTime::create(2),
                CallFunc::create([this](){ setDisableTouchEventForCoopGuide(false); shakeCoop(_coop2Node, 100); _highlight->runAction(FadeOut::create(0.8)); }),
                DelayTime::create(0.8),
                CallFunc::create([this](){ shakeCoop(_coop1Node, 100); }),
            nullptr));

            UserManager::getInstance()->setGuideCoopStatus(guideCoopType::finish);
        }

        GameSoundManager::getInstance()->playBGM("Common/Music/BGM1_TitleScreen_intro.m4a");
        
    }
}

void MainScene::shakeCoop(Node *coop, int times) {
    auto shakeRepeat = Repeat::create(Sequence::create(
        DelayTime::create(1),
        Repeat::create(Sequence::create(
        RotateTo::create(0.05, -3),
        RotateTo::create(0.1, 6),
        RotateTo::create(0.05, -3),
        nullptr), 3),
        nullptr
    ),times);
    coop->runAction(shakeRepeat);
}

void MainScene::setDisableTouchEventForCoopGuide(bool disable) {
    if (disable) {
        _quitButton->setBright(false);
        _quitButton->setTouchEnabled(false);
        _coop1Node->setBright(false);
        _coop1Node->setTouchEnabled(false);
    } else {
        _quitButton->setBright(true);
        _quitButton->setTouchEnabled(true);
        _coop1Node->setBright(true);
        _coop1Node->setTouchEnabled(true);
    }
}


void MainScene::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();

    _transitionBegins = true;
}

void MainScene::zoomCoop(Node *coop)
{
/*
    CoopScene2::CoopType type = CoopScene2::CoopType::CT_MATH;
    
    if (coop==_coop1Node) type = CoopScene2::CoopType::CT_MATH;
    if (coop==_coop2Node) type = CoopScene2::CoopType::CT_LITERACY;
    
    auto scene = CoopScene2::createScene(type);
    scene->setName("CoopScene2");


    auto p1 = _groundNode->convertToWorldSpace(coop->getPosition());
    auto p2 = _rootNode->convertToWorldSpace(designSize/2);
    auto diff = p1 - p2;

    auto coopSpawn = Spawn::create(MoveBy::create(0.5, -(diff*5.0)), ScaleBy::create(0.5, 5), NULL);
    this->runAction(coopSpawn);

    SoundEffect::coopClickEffect().play();

    auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(scene));
    fadeScene->setName("(TransitionFade CoopScene2)");
    Director::getInstance()->pushScene(fadeScene);
/*/
    CoopScene::CoopType type = CoopScene::CoopType::CT_MATH;
    
    if (coop==_coop1Node) type = CoopScene::CoopType::CT_MATH;
    if (coop==_coop2Node) type = CoopScene::CoopType::CT_LITERACY;
    
    auto scene = CoopScene::createScene(type);
    scene->setName("CoopScene");
    
    
    auto p1 = _groundNode->convertToWorldSpace(coop->getPosition());
    auto p2 = _rootNode->convertToWorldSpace(designSize/2);
    auto diff = p1 - p2;
    
    auto coopSpawn = Spawn::create(MoveBy::create(0.5, -(diff*5.0)), ScaleBy::create(0.5, 5), NULL);
    this->runAction(coopSpawn);
    
    SoundEffect::coopClickEffect().play();
    
    auto fadeScene = TransitionFade::create(0.8, TouchEventLogger::wrapScene(scene));
    fadeScene->setName("(TransitionFade CoopScene)");
    Director::getInstance()->pushScene(fadeScene);
//*/
}


void MainScene::confirmDebug()
{
    
    _debugCommand = "";
    
    
    auto pSize = Size(500, 500);
    auto popup = PopupBase::create(this, pSize);
    
    auto textedit = ui::TextField::create("password to debug", "fonts/mukta-bold.ttf", 50);
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
        btn->setTitleText("OK");
        btn->setTitleFontName("fonts/mukta-bold.ttf");
        btn->setTitleColor(Color3B::WHITE);
        btn->setTitleFontSize(50);
        
        btn->addClickEventListener([this, popup, textedit](Ref*) {
            if (textedit->getString()=="2019") {
                popup->dismiss(false);
                UserManager::getInstance()->setDebugMode(true);
                if (this->_debugCoopNo==1)
                    this->zoomCoop(this->_coop1Node);
                else
                    this->zoomCoop(this->_coop2Node);

                
            } else {
                NativeAlert::show("Wrong Password!", "", "OK");
            }
        });
        
        btn->setPosition(Vec2(pSize.width*0.75, pSize.height*0.3));
        popup->addChild(btn);
    }
    
    
    
    
    popup->show(this, true);
    
}

void MainScene::resume() {
    CCLOG("MainScene : resume");
    isDemo = UserDefault::getInstance()->getBoolForKey("review_mode_on", true);
    GameSoundManager::getInstance()->stopBGM();
    MainScene::openAllLevels();
    if (isDemo) {
        _resetBtn->setVisible(true);
        _openAllBtn->setVisible(true);
        _addCoinsBtn->setVisible(true);
        _reviewModeLabel->setVisible(true);
        
    } else {
        _resetBtn->setVisible(false);
        _openAllBtn->setVisible(false);
        _addCoinsBtn->setVisible(false);
        _reviewModeLabel->setVisible(false);
        
    }
    
    _signLanguageModeLabel->setVisible(LanguageManager::getInstance()->isSignLanguageMode());

    _coinTab->updateCoinLabel();
    if (UserManager::getInstance()->getStars() > 0) {
        _coinTab->setVisible(true);
    } else {
        _coinTab->setVisible(false);
    }
}

// function to open all modules
void MainScene::openAllLevels(){
        auto lang = LanguageManager::getInstance()->getCurrentLanguageTag();

        for (auto it : CurriculumManager::getInstance()->levels) {
            LevelCurriculum cur = it.second;
            UserManager::getInstance()->setLevelOpen(cur.levelID);
            UserManager::getInstance()->setPretestProgressType(cur.levelID, PretestProgressType::finish);
            for (int iCounter=0; iCounter<cur.numDays; iCounter++) {
                UserManager::getInstance()->setDayCleared(cur.levelID, iCounter);
            }
        }
}
