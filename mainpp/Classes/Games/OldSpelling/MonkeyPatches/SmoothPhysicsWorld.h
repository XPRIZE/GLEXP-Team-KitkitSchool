//
//  SmoothPhysicsWorld.h -- Physics world with smoothed deltaTime
//  TodoSchool on Aug 24, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/cocos2d.h>


namespace todoschool {

class SmoothPhysicsWorld: public cocos2d::PhysicsWorld {
    typedef cocos2d::PhysicsWorld Super;

public:
    SmoothPhysicsWorld();
    void update(float delta, bool userCall) override;
    
public:
    static SmoothPhysicsWorld* construct(cocos2d::Scene* scene);
    
protected:
    float _cachedUpdateDeltaTime;
};
    
}  // namespace todoschool