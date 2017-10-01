//
//  MainScene.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 6/20/16.
//
//

#include <functional>

#include "CCAppController.hpp"
#include "Utils/TodoUtil.h"


#include "MainScene.hpp"
#include "CoopScene.hpp"
#include "DailyScene.hpp"


#include "GradeSelector.hpp"

#include "Bird.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"
#include "Common/Controls/CompletePopup.hpp"

#include "Managers/LanguageManager.hpp"
#include "Managers/CurriculumManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include "DailyData.hpp"


#include "Common/Effects/FireworksEffect.hpp"


namespace MainSceneSpace {
    const Size designSize = Size(2560,1800);
    static Size visibleSize;
    static Size frameSize;
    
    const Vec2 coopPos = Vec2(designSize.width-220-241, designSize.height-660-172);
    
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
    
    
    ui::Button* coop = ui::Button::create("MainScene/main_coop.png");
    coop->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    coop->setPosition(coopPos);
    coop->addClickEventListener([this](Ref*) {
        if (_transitionBegins) {
            return;
        }
        _transitionBegins = true;

        StrictLogManager::shared()->curriculumChoice_TouchCoop();
        zoomCoop();
    });

    //_groundNode->addChild(coop, coopBackZ);
    _groundNode->addChild(coop);
    _coopNode = coop;
    
    
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
    
    
    _leavesLeft = Sprite::create("MainScene/main_leaves_left.png");
    _leavesLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _leavesLeft->setPosition(Vec2(0, frameSize.height-474));
    _frameNode->addChild(_leavesLeft);
    
    _leavesRight = Sprite::create("MainScene/main_leaves_right.png");
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




void MainScene::onEnter()
{
    Layer::onEnter();
    
    visibleSize = Director::getInstance()->getVisibleSize();

    
    stopAllActions();
    setScale(1.0);
    setPosition(Vec2::ZERO);
    
    //_rootNode->stopAllActions();
    //_rootNode->setScale(_rootScale);
    //_rootNode->setPosition(Vec2(visibleSize.width/2, 0));
    


    _transitionBegins = false;
    
    _coopNode->setPosition(coopPos);
    _coopNode->setScale(1);
   // _groundNode->reorderChild(_coopNode, coopBackZ);
    
 


    
    
    GameSoundManager::getInstance()->stopBGM();
    
    if (__firstEnter)    {
        StrictLogManager::shared()->curriculumChoice_Begin();
        
        __firstEnter = false;
        _transitionBegins = true;
        
        SoundEffect::iconSpawnEffect().preload();
        SoundEffect::coopClickEffect().preload();
        
        const float appearDelay = 2.2;
        
        auto eng = LanguageManager::getInstance()->isEnglish();
        auto logo = Sprite::create(eng ? "System/logo_kitkitschool.png" : "System/logo_kitkitshule.png");
        logo->setPosition(designSize/2);
        _rootNode->addChild(logo);
        logo->runAction(Sequence::create(DelayTime::create(1.7),
                                         EaseIn::create(MoveBy::create(0.2, Vec2(0, -100)), 2.0),
                                         EaseOut::create(MoveBy::create(1.0, Vec2(0, designSize.height)), 2.0),
                                         CallFunc::create([](){ SoundEffect::iconSpawnEffect().play(); }),
                                         DelayTime::create(0.7),
                                         CallFunc::create([this](){
            this->scheduleOnce([this](float){ zoomCoop(); }, 0.01, "zoomCoop");
            //zoomCoop();
        }),
                                         nullptr));
        
        
        _skyNode->setPosition(Vec2(0, -designSize.height*1.0));
        _skyNode->runAction(Sequence::create(DelayTime::create(0.2), MoveTo::create(1.8, Vec2(0, 0)), nullptr));
        
        _groundNode->setPosition(Vec2(0, -designSize.height));
        _groundNode->runAction(Sequence::create(DelayTime::create(1.0),
                                                EaseIn::create( MoveTo::create(1.0, Vec2(0, 0)), 2.0),
                                                nullptr));
        
        _coopNode->runAction(Sequence::create(DelayTime::create(2.0),
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
        
        
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/LogoFall.m4a");

    } else {

        GameSoundManager::getInstance()->playBGM("Common/Music/BGM1_TitleScreen_intro.m4a");

        
    }
    

}

void MainScene::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();

    _transitionBegins = true;
}

void MainScene::zoomCoop()
{
    auto scene = CoopScene::createScene();
    
    
    auto p1 = _groundNode->convertToWorldSpace(coopPos);
    auto p2 = _rootNode->convertToWorldSpace(designSize/2);
    auto diff = p1 - p2;
    
    auto coopSpawn = Spawn::create(MoveBy::create(0.5, -(diff*5.0)), ScaleBy::create(0.5, 5), NULL);
    this->runAction(coopSpawn);
    
    SoundEffect::coopClickEffect().play();
    
    Director::getInstance()->pushScene(TransitionFade::create(0.8, scene));
}


