//
//  TutorialVideoScene.cpp
//  TodoSchool on Jan 07, 2017
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TutorialVideoScene.hpp"
#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoLoadingScene.hpp>
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include "Common/Controls/TutorialVideoPlayer.hpp"

#include <Utils/TodoUtil.h>
#include <cocos/ui/UIButton.h>

#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"

#include "CCAppController.hpp"

using namespace cocos2d;
using namespace std;






TutorialVideoScene::TutorialVideoScene() {
}

bool TutorialVideoScene::init() {
    if (!Super::init()) { return false; }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    setContentSize(winSize);
    
    LayerColor* white = LayerColor::create(cocos2d::Color4B::WHITE,winSize.width, winSize.height);
    addChild(white);
    
    
    _videoNode = Node::create();
    _videoNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoNode->setPosition(winSize/2+Size(0, -50));
    _videoNode->setContentSize(winSize);
    
    addChild(_videoNode);
    
    
    _frame = Sprite::create("TutorialVideo/Xprize_tutorial-video_image_frame.png");
    _frame->setPosition(winSize/2+Size(0, -50));
    
    //    it->setBlendFunc(BlendFunc::DISABLE);
    addChild(_frame);
    

   
    
    
    
    {
        auto skip = ui::Button::create("TutorialVideo/Xprize_tutorial-video_button_close.png");
        
        skip->setPosition(Vec2(winSize.width-150, winSize.height-150));
        skip->setScale(0.6);
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(this->_creator)));
            }
        });
        
    }
    return true;
}


void TutorialVideoScene::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();
    
    auto v = TutorialVideoPlayer::create();
    auto s = v->getContentSize();
    
   // v->setScale(2);
    auto b = v->getBoundingBox();
    
    _videoNode->addChild(v);
    
    auto path = getTutorialFile(_gameName);
    v->playVideo(path);
    v->onCompleted = [this]() {

        Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(this->_creator)));
        
    };
    
}



cocos2d::Scene* TutorialVideoScene::createScene(string gameName, std::function<cocos2d::Scene*(void)> creator) {

    
    auto scene = Scene::create();
    auto winSize = Director::getInstance()->getWinSize();
    
    
    auto node = TutorialVideoScene::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPosition(winSize / 2.f);
    
    node->_creator = creator;
    node->_gameName = gameName;
    

    
    scene->addChild(node);

    
    return scene;
}

bool TutorialVideoScene::tutorialExists(std::string gameName)
{
    
    auto path = getTutorialFile(gameName);
    auto exists = FileUtils::getInstance()->isFileExist(path);
    return exists;
    
    
}

std::string TutorialVideoScene::getTutorialFile(std::string gameName)
{
    auto lang = LanguageManager::getInstance()->getCurrentLanguageCode();
    auto path = "TutorialVideo/"+lang+"/xPrize Tutorial - "+gameName+".m4v";
    return path;
}

