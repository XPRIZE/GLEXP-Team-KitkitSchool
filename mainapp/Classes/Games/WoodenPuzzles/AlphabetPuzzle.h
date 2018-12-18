//
//  AlphabetPuzzle.h on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/2d/CCScene.h>


class AlphabetPuzzle {
    int LevelID;
    std::function<void()> OnSuccess;
    std::function<void()> OnFail;
    
public:
    AlphabetPuzzle();
    AlphabetPuzzle(int LevelID,
                   std::function<void()> OnSuccess,
                   std::function<void()> OnFail);
    
    // NB(xenosoz, 2016): Adaptors for coding convention of upstream.
    std::vector<int> getCandidateLevelIDs();

    void setLevelID(int LevelID);
    void setOnSuccess(std::function<void()> F);
    void setOnFail(std::function<void()> F);
    
    cocos2d::Scene* createScene();
};
