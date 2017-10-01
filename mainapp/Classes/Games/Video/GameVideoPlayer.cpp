//
//  GameVideoPlayer.cpp
//  enumaXprize
//
//  Created by Sungwoo Kang on 9/6/17.
//
//

#include "GameVideoPlayer.hpp"

#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include "Common/Controls/TodoSchoolBackButton.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif


using namespace cocos2d::experimental::ui;

bool GameVideoPlayer::init()
{
    
    if (!Layer::init()) return false;
    
    onCompleted = nullptr;
    
    auto winSize = getContentSize();
    auto videoSize = Size(1280, 720);
    Size videoArea = Size(winSize.width, winSize.height-360);
    float videoScale = MIN(videoArea.width / videoSize.width, videoArea.height / videoSize.height);
    
    
    
    
    
    auto bg = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
    addChild(bg);
    
    _player = VideoPlayer::create();
    
    _player->setContentSize(videoSize * videoScale);
    _player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _player->setPosition(winSize/2);
    
    _player->setTouchEnabled(false);
    //    _player->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
    //        if (e == ui::Widget::TouchEventType::ENDED) {
    //            if (_player->isPlaying()) {
    //                _player->pause();
    //            }
    //            else {
    //                _player->play();
    //            }
    //        }
    //
    //    });
    
    _player->addEventListener([this](Ref*, VideoPlayer::EventType E) {
        switch (E) {
            case VideoPlayer::EventType::PLAYING:
            case VideoPlayer::EventType::PAUSED:
            case VideoPlayer::EventType::STOPPED:
                break;
            case VideoPlayer::EventType::COMPLETED: {
                
                
                handleVideoComplete();
                
                break;
            }
        }
    });
    
    addChild(_player);
    
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = [this] (cocos2d::Touch* touch, cocos2d::Event*) {
//        auto touchPos = convertTouchToNodeSpace(touch);
//        if (_player->getBoundingBox().containsPoint(touchPos)) {
//            if (_player->isPlaying()) {
//                _player->pause();
//            }
//            else {
//                _player->play();
//            }
//            return true;
//        }
//        return false;
//        
//    };
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    
    
    return true;
}

void GameVideoPlayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
    
    _player->removeFromParent();
    this->stopAllActions();
    
}


void GameVideoPlayer::playVideo(string filename)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string uri = getResourceUri(filename);

    if (uri != "") {

        _player->setURL(uri);
        _player->play();

    } else {
        handleVideoComplete();
    }
#else
    if (FileUtils::getInstance()->isFileExist(filename)) {
        _player->setFileName(filename);
        _player->play();
        
    } else {
        handleVideoComplete();
    }
    
#endif
}

void GameVideoPlayer::handleVideoComplete()
{
    auto seq = Sequence::create(FadeOut::create(3.0), CallFunc::create([this](){
        auto Guard = todoschool::NodeScopeGuard(this);
        this->removeFromParent();
        if (onCompleted) {
            onCompleted();
        }
    }), NULL);
    
    this->runAction(seq);
}

string GameVideoPlayer::getResourceUri(string filename)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getResourceUri", "(Ljava/lang/String;)Ljava/lang/String;"))
    {
        jstring jFilename = t.env->NewStringUTF(filename.c_str());
        jstring jUri = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID, jFilename);
        string uri = JniHelper::jstring2string(jUri);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jFilename);

        return uri;
    }
#endif
}
