//
//  TargetTouchBody.h on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "cocos2d.h"
#include "Common/ADT/Chain.h"
#include "Common/ADT/Optional.h"
#include "Common/ADT/FunctionVector.h"
#include <string>


USING_NS_CC;

class TargetTouchBody: public Component {
    typedef Component Super;
    typedef TargetTouchBody ThisType;

    const Camera* CachedHitCamera;
    Chain<bool> Highlighted;

public:
    FunctionVector<bool(Touch*, Event*)> OnTouchDidBegin;
    FunctionVector<void(Touch*, Event*)> OnTouchDidMove;
    FunctionVector<void(Touch*, Event*)> OnTouchDidEnd;
    FunctionVector<void(Touch*, Event*)> OnTouchDidCancel;

public:
    CREATE_FUNC(TargetTouchBody);
    TargetTouchBody();
    bool init() override;
    
    static std::string defaultName();

private:
    void onAdd() override;
    void onRemove() override;
    
    bool handleTouchDidBegin(Touch* T, Event* E);
    void handleTouchDidMove(Touch* T, Event* E);
    void handleTouchDidEnd(Touch* T, Event* E);
    void handleTouchDidCancel(Touch* T, Event* E);

    Optional<Vec3> containsPointInContentRectWithCamera(const Vec2& P, const Camera* C) const;
    bool clippedOutByAncestors(Point P);
};

