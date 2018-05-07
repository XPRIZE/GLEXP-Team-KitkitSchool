//
//  GameVideoScene.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 9/6/17.
//
//

#include "GameVideoScene.hpp"

#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoLoadingScene.hpp>
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include "GameVideoPlayer.hpp"

#include <Utils/TodoUtil.h>
#include <cocos/ui/UIButton.h>

#include "Managers/UserManager.hpp"
#include "Managers/LanguageManager.hpp"

#include "CCAppController.hpp"
#include "Common/Controls/CompletePopup.hpp"

using namespace cocos2d;
using namespace std;






GameVideoScene::GameVideoScene() {
}

bool GameVideoScene::init() {
    if (!Super::init()) { return false; }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    setContentSize(winSize);
    
    _videoNode = Node::create();
    _videoNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoNode->setPosition(winSize/2);
    _videoNode->setContentSize(winSize);
    
    addChild(_videoNode);
    
    
    _frame = Sprite::create("TutorialVideo/video_frame.jpg");
    _frame->setPosition(winSize/2);
    
    //    it->setBlendFunc(BlendFunc::DISABLE);
    addChild(_frame);
    
    
    
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, winSize.height-25));
    addChild(backButton);

    
    {
//        auto skip = ui::Button::create("TutorialVideo/video_button_skip.png");
        
//        skip->setPosition(Vec2(2560-170, 1800-50));
//        skip->setScale(1.0/0.9);
//        _frame->addChild(skip);
//        skip->addTouchEventListener([](Ref*,ui::Widget::TouchEventType e) {
//            if (e == ui::Widget::TouchEventType::ENDED) {
//                Director::getInstance()->popScene();
//                Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(this->_creator)));
//            }
//        });
        
    }
    return true;
}


void GameVideoScene::onEnterTransitionDidFinish()
{
    Super::onEnterTransitionDidFinish();
    
    auto v = GameVideoPlayer::create();
    auto s = v->getContentSize();
    
    // v->setScale(2);
    auto b = v->getBoundingBox();
    
    _videoNode->addChild(v);
    
    auto path = getVideoFile(_filename);
    v->playVideo(path);
    v->onCompleted = [this]() {
        auto CP = CompletePopup::create();
        
        CP->show(1.5f, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
        // Director::getInstance()->popScene();
        // Director::getInstance()->replaceScene(TransitionFade::create(0.5, TodoLoadingScene::createScene(this->_creator)));
    };
    
}



cocos2d::Scene* GameVideoScene::createScene(string filename) {
    
    
    auto scene = Scene::create();
    auto winSize = Director::getInstance()->getWinSize();
    
    
    auto node = GameVideoScene::create();
    node->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    node->setPosition(winSize / 2.f);
    
    node->_filename = filename;
    
    
    
    scene->addChild(node);
    
    
    return scene;
}

bool GameVideoScene::videoExists(std::string filename)
{
    
    auto path = getVideoFile(filename);
    auto exists = FileUtils::getInstance()->isFileExist(path);
    return exists;
    
    
}

std::string GameVideoScene::getVideoFile(std::string filename)
{
    auto path = "Videos/"+filename+".mp4";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path = filename;
#endif

    return path;
}
