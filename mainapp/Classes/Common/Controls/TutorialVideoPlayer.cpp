//
//  TutorialVideoPlayer.cpp
//  enumaXprize
//
//  Created by Gunho Lee on 12/20/16.
//
//

#include "TutorialVideoPlayer.hpp"
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include "Common/Controls/TodoSchoolBackButton.hpp"


using namespace cocos2d::experimental::ui;

bool TutorialVideoPlayer::init()
{

    if (!Layer::init()) return false;
    
    onCompleted = nullptr;
    
    auto winSize = getContentSize();
    auto videoSize = Size(1682-76, 1207-76);
    

    auto bg = LayerColor::create(Color4B(0, 0, 0, 0), winSize.width, winSize.height);
    addChild(bg);
    
    _player = VideoPlayer::create();

    _player->setContentSize(videoSize);
    _player->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _player->setPosition(winSize/2);
    
    _player->setTouchEnabled(false);

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
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [] (cocos2d::Touch*, cocos2d::Event*) {
        return true;
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg);
    

    
    
    return true;
}

void TutorialVideoPlayer::onExitTransitionDidStart()
{
    Layer::onExitTransitionDidStart();
    
    _player->removeFromParent();
    this->stopAllActions();
    
}


void TutorialVideoPlayer::playVideo(string filename)
{

    
    if (FileUtils::getInstance()->isFileExist(filename)) {
        _player->setFileName(filename);
        _player->play();
        
    } else {
        handleVideoComplete();
    }
    
}

void TutorialVideoPlayer::handleVideoComplete()
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
