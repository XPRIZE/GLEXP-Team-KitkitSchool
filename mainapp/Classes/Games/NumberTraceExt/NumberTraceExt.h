//
//  NumberTraceExt.h -- Word tracing game with bonus videos
//  TodoSchool on May 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/2d/CCScene.h>
#include <functional>
#include <string>

class NumberTraceExt
{
    std::function<void()> OnSuccess;
    std::function<void()> OnFail;

public:
    NumberTraceExt();
    void setOnSuccess(std::function<void()> F);
    void setOnFail(std::function<void()> F);
    cocos2d::Scene* createScene(int levelID);
};
