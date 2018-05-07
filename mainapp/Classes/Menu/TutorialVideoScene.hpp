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

class TutorialVideoScene: public cocos2d::Node
{
public:
    typedef cocos2d::Node Super;
    std::function<cocos2d::Scene*(void)> _creator;

    Node *_skipBtn;
    
private:
    std::string _gameName;
    std::string _fileName;
    Node *_videoNode;
    cocos2d::Sprite *_frame;

public:
    CREATE_FUNC(TutorialVideoScene);
    TutorialVideoScene();
    virtual bool init() override;
    virtual void onEnterTransitionDidFinish() override;
    
public:
    // NB(xenosoz, 2016): Create a minimal scene containing a GradeSelector.
    static cocos2d::Scene* createScene(std::string gameName, std::function<cocos2d::Scene*(void)> creator, bool allowSkip = true);
    static cocos2d::Scene* createSceneWithCustomFileName(std::string fileName, std::function<cocos2d::Scene*(void)> creator, bool allowSkip = true);
    static bool tutorialExists(std::string gameName);
    static bool tutorialExistsWithCustomName(std::string fileName);
    static std::string getTutorialFile(std::string gameName);
    static std::string getTutorialFileWithCustomName(std::string filename);
};
