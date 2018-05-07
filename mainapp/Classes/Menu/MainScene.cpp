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
#include "CoopScene2.hpp"



#include "GradeSelector.hpp"

#include "Bird.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/CompletePopup.hpp"
#include "Common/Controls/PopupBase.hpp"
#include "Common/Controls/TouchEventLogger.h"


#include "Managers/LanguageManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"



#include "Common/Effects/FireworksEffect.hpp"

#include "3rdParty/CCNativeAlert.h"


namespace MainSceneSpace {
    const Size designSize = Size(2560,1800);
    static Size visibleSize;
    static Size frameSize;
    
    const Vec2 coop1Pos = Vec2(designSize.width-220-241, designSize.height-660-172);
    const Vec2 coop2Pos = Vec2(designSize.width-220-241-550, designSize.height-660-172);
    
    //static Vec2 quitPos;
    
    const Size quitNodeSize = Size(223, 158);
    
  //  const int coopBackZ = -100;

    
    const string fontName = "fonts/TodoMainCurly.ttf";
    
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
        if (_transitionBegins) {
            return;
        }
        
        if (_debugCommand=="lrllrlrr") {
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
        if (_transitionBegins) {
            return;
        }
        
        if (_debugCommand=="lrllrlrr") {
            this->confirmDebug();
            return;
        }
        
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
                UserManager::getInstance()->sendAppToBack();
                
            }
        };
        _quitButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, _quitButton);
        _quitButton->addClickEventListener([this](Ref*){
            StrictLogManager::shared()->curriculumChoice_End();
            UserManager::getInstance()->sendAppToBack();
        });

    }
    
    
    
    
    SoundEffect::coopClickEffect().preload();
    

    

    
    
    return true;
}

void MainScene::manageCache() {
    
    vector<string> fileList = {
        "MainScene/DailyScene/daily_bg_large.png",
        "MainScene/DailyScene/daily_birdshadow.png",
        "MainScene/DailyScene/daily_bluebutton_active.png",
        "MainScene/DailyScene/daily_bluebutton_effect_glow.png",
        "MainScene/DailyScene/daily_bluebutton_normal.png",
        "MainScene/DailyScene/daily_coinstatus_bg.png",
        "MainScene/DailyScene/daily_freechoice_circleicon_available.png",
        "MainScene/DailyScene/daily_freechoice_circleicon_new.png",
        "MainScene/DailyScene/daily_freechoice_circleicon_new_effect_1.png",
        "MainScene/DailyScene/daily_freechoice_circleicon_new_effect_2.png",
        "MainScene/DailyScene/daily_freechoice_circleicon_unavailable.png",
        "MainScene/DailyScene/daily_freechoice_close_active.png",
        "MainScene/DailyScene/daily_freechoice_close_normal.png",
        "MainScene/DailyScene/daily_freechoice_popup_icon_done.png",
        "MainScene/DailyScene/daily_freechoice_popup_level_active_glow.png",
        "MainScene/DailyScene/daily_freechoice_popup_level_done.png",
        "MainScene/DailyScene/daily_freechoice_popup_level_normal.png",
        "MainScene/DailyScene/daily_freechoice_popup_level_unavailable.png",
        "MainScene/DailyScene/daily_freechoice_popup_panel.png",
        //"MainScene/DailyScene/daily_freechoice_popup_shade_bg.png",
        "MainScene/DailyScene/daily_freechoice_popup_window.png",
        "MainScene/DailyScene/daily_panel_literacy.png",
        "MainScene/DailyScene/daily_panel_math.png",
        "MainScene/DailyScene/daily_panel_prek.png",
        "MainScene/DailyScene/daily_treetop_right.png",
        "MainScene/DailyScene/daily_treetrunk.png",
        "MainScene/DailyScene/freechoice_icon_disabled.png",
        "MainScene/DailyScene/freechoice_icon_new.png",
        "MainScene/DailyScene/freechoice_icon_new_bg.png",
        "MainScene/DailyScene/freechoice_icon_new_effect.png",
        "MainScene/DailyScene/freechoice_icon_normal.png",
        "MainScene/DailyScene/freechoice_icon_normal_bg.png",
        "MainScene/DailyScene/freechoice_popup_bg.png",
        "MainScene/DailyScene/freechoice_popup_button_close.png",
        "MainScene/DailyScene/freechoice_popup_button_open.png"
        /*"BirdAnimation/coop_egg_english_1.png",
        "BirdAnimation/coop_egg_english_2.png",
        "BirdAnimation/coop_egg_english_3.png",
        "BirdAnimation/coop_egg_english_4.png",
        "BirdAnimation/coop_egg_english_5.png",
        "BirdAnimation/coop_egg_english_6.png",
        "BirdAnimation/coop_egg_english_7.png",
        "BirdAnimation/coop_egg_english_8.png",
        "BirdAnimation/coop_egg_math_1.png",
        "BirdAnimation/coop_egg_math_2.png",
        "BirdAnimation/coop_egg_math_3.png",
        "BirdAnimation/coop_egg_math_4.png",
        "BirdAnimation/coop_egg_math_5.png",
        "BirdAnimation/coop_egg_math_6.png",
        "BirdAnimation/coop_egg_math_7.png",
        "BirdAnimation/coop_egg_math_8.png",
        "BirdAnimation/egg_crack.png",
        
        "MainScene/FreeChoiceThumbnail/freechoice_game_AbcBook.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_AlphabetPuzzle.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_AnimalPuzzle.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_BirdPhonics.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_Book.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_BubblePop.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_Comprehension.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_Counting.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_DoubleDigit.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_EquationMaker.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_FeedingTime.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_FindTheMatch.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_FishTank.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_HundredPuzzle.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_LetterMatching.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_LetterTrace.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_LetterTracingCard.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_LineMatching.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_MangoShop.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_MissingNumber.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_MovingInsects.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_MultiTrace.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_NumberMatching.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_NumberPuzzle.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_NumberTracing.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_NumberTracingExt.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_NumberTrain.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_PatternTrain.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_ReadingBird.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_SentenceMaker.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_ShapeMatching.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_ShowAndTell.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_SoundTrain.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_Spelling.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_StarFall.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_Tapping.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_ThirtyPuzzle.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_TutorialTrace.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_WhatIsThis.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_WordMachine.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_WordNote.png",
        "MainScene/FreeChoiceThumbnail/freechoice_game_WordTracing.png"*/
    };
    /*
    vector<string> directoryList = {
        "MainScene",
        "CoopScene",
        "MainScene/DailyScene",
        "BirdAnimation",
        "MainScene/FreeChoiceThumbnail"
    };
    
    for (auto it : directoryList) {
        for (auto f : FileUtils::getInstance()->listFiles(it)) {
            if (f.find("jpg") == string::npos && f.find(".png") == string::npos) continue;
            fileList.push_back(f);
        }
    }
    */
    
    for (auto it : CurriculumManager::getInstance()->levels) {
        LevelCurriculum cur = it.second;
        if (!UserManager::getInstance()->isLevelOpen(cur.levelID)) continue;
        auto bird = Bird::create(cur.category, cur.categoryLevel, cur.levelID);
        bird->loadAnimation();
    }
    
    for (auto it : fileList) {
        if (!FileUtils::getInstance()->isFileExist(it)) continue;
        Director::getInstance()->getTextureCache()->addImage(it);
        CCLOG("add cache: %s", it.c_str());
    }
    
}

void MainScene::onEnter()
{
    Layer::onEnter();
    
    visibleSize = Director::getInstance()->getVisibleSize();

    _debugCommand = "";
    
    stopAllActions();
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
    
 


    
    
    GameSoundManager::getInstance()->stopBGM();
    
    if (__firstEnter)    {
        StrictLogManager::shared()->curriculumChoice_Begin();
        
        __firstEnter = false;
        _transitionBegins = true;
        
        SoundEffect::iconSpawnEffect().preload();
        SoundEffect::coopClickEffect().preload();
        
        const float appearDelay = 2.2;
        
        //auto eng = LanguageManager::getInstance()->isEnglish();
        auto logoPath = LanguageManager::getInstance()->findLocalizedResource("System/logo_phase02.png");
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
        
        _coop1Node->runAction(Sequence::create(DelayTime::create(2.0),
                                              EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
                                              EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
                                              nullptr));
        _coop2Node->runAction(Sequence::create(DelayTime::create(2.2),
                                               EaseOut::create(MoveBy::create(0.12, Vec2(0, 50)), 2.0),
                                               EaseIn::create(MoveBy::create(0.12, Vec2(0, -50)), 2.0),
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
                                                 nullptr));
        
        
        
//        _quitButton->setPosition(Vec2(designSize.width+quitNodeSize.width, quitPos.y));
//        _quitButton->runAction(Sequence::create(
//                                               DelayTime::create(2.5),
//                                               EaseOut::create(MoveTo::create(0.2, quitPos), 2.0),
//                                                
//                                               nullptr));
        
        GameSoundManager::getInstance()->playEffectSoundForAutoStart("Common/Sounds/Effect/LogoFall.m4a");

    } else {

        GameSoundManager::getInstance()->playBGM("Common/Music/BGM1_TitleScreen_intro.m4a");
        
    }

}

void MainScene::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();

    _transitionBegins = true;
}

void MainScene::zoomCoop(Node *coop)
{
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
}


void MainScene::confirmDebug()
{
    
    _debugCommand = "";
    
    
    auto pSize = Size(500, 500);
    auto popup = PopupBase::create(this, pSize);
    
    auto textedit = ui::TextField::create("password to debug", "fonts/OpenSans-Bold.ttf", 50);
    textedit->setPosition(Vec2(pSize.width*0.5, pSize.height*0.7));
    popup->addChild(textedit);
    
    {
        auto btn = ui::Button::create();
        btn->setTitleText("Cancel");
        btn->setTitleFontName("fonts/OpenSans-Bold.ttf");
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
        btn->setTitleFontName("fonts/OpenSans-Bold.ttf");
        btn->setTitleColor(Color3B::WHITE);
        btn->setTitleFontSize(50);
        
        btn->addClickEventListener([this, popup, textedit](Ref*) {
            if (textedit->getString()=="2019") {
                popup->dismiss(false);
                UserManager::getInstance()->setDebugMode(true);
                this->zoomCoop(this->_coop1Node);

                
            } else {
                NativeAlert::show("Wrong Password!", "", "OK");
            }
        });
        
        btn->setPosition(Vec2(pSize.width*0.75, pSize.height*0.3));
        popup->addChild(btn);
    }
    
    
    
    
    popup->show(this, true);
    
}
