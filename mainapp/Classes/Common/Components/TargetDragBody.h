//
//  TargetDragBody.h on Aug 16, 2016
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

class TargetDragBody: public Component {
    typedef Component Super;
    
public:
    FunctionVector<bool(Touch*, Event*)> OnDragDidBegin;
    FunctionVector<void(Touch*, Event*)> OnDrag;
    FunctionVector<void(Touch*, Event*)> OnDragDidEnd;
    
public:
    CREATE_FUNC(TargetDragBody);
    TargetDragBody();
    bool init() override;
    
    static std::string defaultName();
    
private:
    void onAdd() override;
    void onRemove() override;
};


