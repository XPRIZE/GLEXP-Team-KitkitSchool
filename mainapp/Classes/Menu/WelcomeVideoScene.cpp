//
//  WelcomeVideoScene.cpp
//  TodoSchool on Jan 07, 2017
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WelcomeVideoScene.hpp"

#include <Utils/TodoUtil.h>
#include <cocos/ui/UIButton.h>

#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"

#include "Menu/MainScene.hpp"
#include "Common/Controls/TodoLoadingScene.hpp"

#include "CCAppController.hpp"

using namespace cocos2d;
using namespace std;

WelcomeVideoScene::WelcomeVideoScene() {
}

bool WelcomeVideoScene::init() {
    if (!Super::init()) { return false; }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    setContentSize(winSize);
    
    LayerColor* background = LayerColor::create(cocos2d::Color4B::BLACK,winSize.width, winSize.height);
    addChild(background);
    
    _videoNode = Node::create();
    _videoNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoNode->setPosition(winSize/2+Size(0, -50));
    _videoNode->setContentSize(winSize);
    addChild(_videoNode);
    return true;
}


void WelcomeVideoScene::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();

    auto winSize = Director::getInstance()->getWinSize();
    _player = KitkitVideoPlayer::create();
    
    float ratio = MIN(winSize.width / 960, winSize.height / 540);

    _player->setContentSize(Size(960,540));
    _player->setScale(ratio);
    _player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _player->setPosition(Vec2(winSize.width/2, winSize.height/2+(20*ratio)));

    _player->setTouchEnabled(false);
    
    _player->setFileName("TutorialVideo/vdo_phase3_opening_movie.mp4");
    _player->play();
    
    UserDefault::getInstance()->setBoolForKey(UserManager::getInstance()->getWelcomeVideoPlayedKey().c_str(), true);
    
    _player->addEventListener([this](Ref*, VideoPlayer::EventType E) {
        switch (E) {
            case VideoPlayer::EventType::PLAYING:
            case VideoPlayer::EventType::PAUSED:
            case VideoPlayer::EventType::STOPPED:
                break;
            case VideoPlayer::EventType::COMPLETED: {
                _player->setVisible(false);
                auto seq = Sequence::create(FadeOut::create(0.2), CallFunc::create([this](){
                    
                    
                    std::function<Scene*(void)> creator = []() {
                        auto scene = MainScene::createScene();
                        scene->setName("MainScene");
                        return scene;
                    };
                    Director::getInstance()->replaceScene(TransitionFade::create(1.f, TodoLoadingScene::createScene(creator)));
                    
                    
               }), NULL);
                this->runAction(seq);
                break;
            }
        }
    });
    
    _videoNode->addChild(_player);
    
    
    
}

cocos2d::Scene* WelcomeVideoScene::createScene() {
    
    auto scene = Scene::create();
    auto winSize = Director::getInstance()->getWinSize();
    
    
    auto node = WelcomeVideoScene::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPosition(winSize / 2.f);
    
    scene->addChild(node);
    return scene;
}
