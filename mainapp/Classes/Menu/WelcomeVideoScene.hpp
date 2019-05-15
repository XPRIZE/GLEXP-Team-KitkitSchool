//
//  WelcomeVideoScene.hpp
//  TodoSchool on Jan 07, 2017
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/cocos2d.h>
#include <vector>
#include <functional>
#include <string>

#include "Common/Controls/KitkitVideoPlayer.hpp"

class WelcomeVideoScene: public cocos2d::Node {
public:
    typedef cocos2d::Node Super;
    
    std::function<cocos2d::Scene*(void)> _creator;
    
private:
    std::string _gameName;
    
    
    Node *_videoNode;
    cocos2d::Sprite *_frame;
    KitkitVideoPlayer *_player;
    
public:
    CREATE_FUNC(WelcomeVideoScene);
    WelcomeVideoScene();
    virtual bool init() override;
    
    //virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    
public:
    // NB(xenosoz, 2016): Create a minimal scene containing a GradeSelector.
    static cocos2d::Scene* createScene();
    
};

