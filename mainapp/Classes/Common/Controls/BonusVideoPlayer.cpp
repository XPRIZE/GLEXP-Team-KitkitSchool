//
//  BonusVideoPlayer.cpp on May 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "BonusVideoPlayer.h"
#include "Common/Basic/DeviceSpec.h"
#include "Common/Basic/NodeScopeGuard.h"
#include <cocos/cocos2d.h>
#include <string>
#include <functional>


using namespace cocos2d;
using namespace std;




namespace {
    // XXX: Do we need the default video file?
    string DefaultVideoFilePath = "LetterTrace/BonusVideos/empty.m4v";
}


namespace todoschool {

BonusVideoPlayer::BonusVideoPlayer()
: TheVideoPlayer(nullptr)
{
}

bool BonusVideoPlayer::init() {
    if (!Super::init()) { return false; }
    
    clear();
    return true;
}

void BonusVideoPlayer::setContentSize(const Size& ContentSize) {
    Super::setContentSize(ContentSize);

    TheVideoPlayer->setContentSize(ContentSize);
}

void BonusVideoPlayer::setVisible(bool Visible) {
    Super::setVisible(Visible);

    TheVideoPlayer->setVisible(Visible);
}

void BonusVideoPlayer::clear() {
    clearInternals();
    
    Size GameSize = DeviceSpec::Google_Pixel_C().FullScreenResolution;
    //Size WindowSize = DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
    
    if (TheVideoPlayer)
        TheVideoPlayer->removeFromParent();
    
    TheVideoPlayer = ([&]{
        auto It = KitkitVideoPlayer::create();
        It->setContentSize(GameSize);
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point::ZERO);

        It->setTouchEnabled(false);
        It->setFileName(::DefaultVideoFilePath);
        It->addEventListener(std::bind(&BonusVideoPlayer::handleVideoPlayerStateUpdated, this, placeholders::_1, placeholders::_2));
        addChild(It);
        return It;
    }());
}

void BonusVideoPlayer::play() {
    
    auto file = TheVideoPlayer->getFileName();
    if (FileUtils::getInstance()->isFileExist(file)) {
        TheVideoPlayer->play();
    } else {
        if (OnPlayDidEnd) {
        
            runAction(Sequence::create(DelayTime::create(5.0), CallFunc::create([this]() {
                OnPlayDidEnd(this);
            }), nullptr));
            
            
        }
    }
}
    
void BonusVideoPlayer::stop() {
    TheVideoPlayer->stop();
}

void BonusVideoPlayer::clearInternals() {
    VideoFilePath.OnValueUpdate = [this](string&)
    {
        if (!TheVideoPlayer) { return; }

        CCLOG("Try to set video file path to: %s", VideoFilePath().c_str());
        string P = (FileUtils::getInstance()->isFileExist(VideoFilePath())
                           ? VideoFilePath()
                           : DefaultVideoFilePath);

        CCLOG("Set video file path to: %s", P.c_str());
        TheVideoPlayer->setFileName(P);
    };
}

void BonusVideoPlayer::handleVideoPlayerStateUpdated(Ref*, VideoPlayer::EventType E) {
    auto Guard = NodeScopeGuard(this);

    switch (E) {
        case VideoPlayer::EventType::PLAYING: {
            if (OnPlayDidBegin)
                OnPlayDidBegin(this);
            break;
        }
        case VideoPlayer::EventType::PAUSED: {
            if (OnPlayDidPause)
                OnPlayDidPause(this);
            break;
        }
        case VideoPlayer::EventType::STOPPED: {
            if (OnPlayDidStop)
                OnPlayDidStop(this);
            break;
        }
        case VideoPlayer::EventType::COMPLETED: {
            if (OnPlayDidEnd)
                OnPlayDidEnd(this);
            break;
        }
    }
}

}  // namespace todoschool
