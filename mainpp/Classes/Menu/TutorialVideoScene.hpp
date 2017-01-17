//
//  TutorialVideoScene.hpp
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


class TutorialVideoScene: public cocos2d::Node {
public:
    typedef cocos2d::Node Super;

    std::function<cocos2d::Scene*(void)> _creator;

private:
    std::string _gameName;
    
    
    Node *_videoNode;
    cocos2d::Sprite *_frame;
    

public:
    CREATE_FUNC(TutorialVideoScene);
    TutorialVideoScene();
    virtual bool init() override;

    //virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;
    
    
public:
    // NB(xenosoz, 2016): Create a minimal scene containing a GradeSelector.
    static cocos2d::Scene* createScene(std::string gameName, std::function<cocos2d::Scene*(void)> creator);

    static bool tutorialExists(std::string gameName);
    static std::string getTutorialFile(std::string gameName);
    

};
