//
//  CompTrace.h -- Word tracing game with bonus videos
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/2d/CCScene.h>
#include <functional>
#include <string>
#include "../ComprehensionScene.hpp"
class CompTrace {

    

    
    std::function<void()> OnSuccess;
    std::function<void()> OnFail;

public:
    CompTrace();


    void setOnSuccess(std::function<void()> F);
    void setOnFail(std::function<void()> F);

    cocos2d::Scene* createScene();
    cocos2d::Layer* createLayer(ComprehensionScene* parent);
    
};
