//
//  RawTouchBody.h on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "cocos2d.h"
#include "Common/ADT/FunctionVector.h"
#include <string>


USING_NS_CC;



class RawTouchBody: public Component {
    typedef Component Super;
    EventListener* Listener;
    
public:
    FunctionVector<bool(Touch*, Event*)> OnTouchDidBegin;
    FunctionVector<void(Touch*, Event*)> OnTouchDidMove;
    FunctionVector<void(Touch*, Event*)> OnTouchDidEnd;
    FunctionVector<void(Touch*, Event*)> OnTouchDidCancel;
    
public:
    CREATE_FUNC(RawTouchBody);
    RawTouchBody();
    bool init() override;

    static std::string defaultName();

private:
    void onEnter() override;
    void onExit() override;
};


