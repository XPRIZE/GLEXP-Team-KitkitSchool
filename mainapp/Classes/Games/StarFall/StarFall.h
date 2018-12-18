//
//  StarFall.h -- Bubble-falling typing game with a keyboard
//  TodoSchool on Jul 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/2d/CCScene.h>
#include <functional>


class StarFall {
    size_t LevelID;
    std::function<void()> OnSuccess;
    std::function<void()> OnFail;

public:
    StarFall();
    StarFall(int LevelID,
             std::function<void()> OnSuccess,
             std::function<void()> OnFail);
    
    // NB(xenosoz, 2016): Adaptors for coding convention of upstream.
    std::vector<int> getCandidateLevelIDs();

    void setLevelID(int LevelID);
    void setOnSuccess(std::function<void()> F);
    void setOnFail(std::function<void()> F);
    
    cocos2d::Scene* createScene();
};
