//
//  SignLanguageVideoPlayer.cpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 5/11/18.
//

#include "SignLanguageVideoPlayer.hpp"

#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/NodeScopeGuard.h"
#include "KitkitVideoPlayer.hpp"
#include "Utils/TodoUtil.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

#define BOTTOM_LEFT_MARGIN      20
#define VIDEO_SIZE              Size(500.f, 667.f)

SignLanguageVideoPlayer *SignLanguageVideoPlayer::currentPlayer = nullptr;

void SignLanguageVideoPlayer::playVideoIfSignLanguageMode(Node* parent, string videoPath, function<void()> callback)
{
    auto isSignLanguageMode = UserDefault::getInstance()->getBoolForKey("sign_language_mode_on", false);
    if (isSignLanguageMode == false)
        return;
    
    if (currentPlayer != nullptr)
    {
        currentPlayer->stopAllActions();
        currentPlayer->removeAllChildren();
        currentPlayer->removeFromParent();
        currentPlayer = nullptr;
    }
    
    auto videoPlayer = SignLanguageVideoPlayer::create();
    parent->addChild(videoPlayer);
    currentPlayer = videoPlayer;
    videoPlayer->playVideo(videoPath);
    videoPlayer->onCompleted = [videoPlayer, parent, callback](){
        videoPlayer->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([videoPlayer, parent, callback](){
            if (videoPlayer != nullptr)
            {
                if (currentPlayer == videoPlayer)
                    currentPlayer = nullptr;
                videoPlayer->removeAllChildren();
                videoPlayer->removeFromParent();
                
                if (callback != nullptr)
                    callback();
            }
        }), nullptr));
    };
}

bool SignLanguageVideoPlayer::init()
{
    if (!Layer::init()) return false;
    
    onCompleted = nullptr;
    
    this->setContentSize(Director::getInstance()->getVisibleSize());
    
    _player = KitkitVideoPlayer::create();
    _player->setContentSize(VIDEO_SIZE);
    _player->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _player->setPosition(Vec2(BOTTOM_LEFT_MARGIN, BOTTOM_LEFT_MARGIN));
    _player->setTouchEnabled(false);
    _player->addEventListener([this](Ref*, VideoPlayer::EventType E) {
        switch (E)
        {
            case VideoPlayer::EventType::PLAYING:
            case VideoPlayer::EventType::PAUSED:
            case VideoPlayer::EventType::STOPPED:
                break;
            case VideoPlayer::EventType::COMPLETED:
            {
                handleVideoComplete();
                break;
            }
        }
    });
    addChild(_player);
    return true;
}

void SignLanguageVideoPlayer::playVideo(string filename)
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

/*
void SignLanguageVideoPlayer::playVideo(string filename)
{
    if (FileUtils::getInstance()->isFileExist(filename))
    {
        _player->setFileName(filename);
        _player->play();
    }
    else
    {
        handleVideoComplete();
    }
}
*/

void SignLanguageVideoPlayer::handleVideoComplete()
{
    if (onCompleted)
    {
        onCompleted();
    }
    
    /*
    auto seq = Sequence::create(FadeOut::create(3.0), CallFunc::create([this]() {
        auto Guard = NodeScopeGuard(this);
        this->removeFromParent();
        if (onCompleted)
        {
            onCompleted();
        }
    }), nullptr);
    this->runAction(seq);
     */
}

string SignLanguageVideoPlayer::getResourceUri(string filename)
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
    return filename;
}

void SignLanguageVideoPlayer::onExit()
{
    Layer::onExit();
    currentPlayer = nullptr;
}
