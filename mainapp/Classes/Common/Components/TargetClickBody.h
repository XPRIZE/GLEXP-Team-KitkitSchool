//
//  TargetClickBody.h on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "cocos2d.h"
#include "Common/ADT/FunctionVector.h"

USING_NS_CC;




class TargetClickBody: public Component {
    typedef Component Super;
    typedef TargetClickBody ThisType;

public:
    FunctionVector<void(Touch*, Event*)> OnClick;
    
public:
    CREATE_FUNC(TargetClickBody);
    TargetClickBody();
    bool init() override;
    
    static std::string defaultName();
    
private:
    void onAdd() override;
    void onRemove() override;
    
    bool handleTouchDidBegin(Touch* T, Event* E);
    void handleTouchDidMove(Touch* T, Event* E);
    void handleTouchDidEnd(Touch* T, Event* E);
    void handleTouchDidCancel(Touch* T, Event* E);
};

