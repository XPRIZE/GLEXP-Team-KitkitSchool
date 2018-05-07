//
//  TouchEventLogger.h on Apr 3, 2018
//  TodoSchool
//
//  Copyright (c) 2018 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include <cocos/cocos2d.h>
#include <string>

class TouchEventLogger: public cocos2d::Node {
public:
    typedef cocos2d::Node Super;
    std::string _parentName;
    cocos2d::EventListener* _listener;
    
public:
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;

public:
    TouchEventLogger(std::string parentName);
    static TouchEventLogger* create(std::string parentName);
    static cocos2d::Scene* wrapScene(cocos2d::Scene* scene);
};

//EOF
