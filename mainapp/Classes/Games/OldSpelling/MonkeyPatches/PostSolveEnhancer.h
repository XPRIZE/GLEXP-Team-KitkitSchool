//
//  PostSolveEnhancer.h -- Extract more information from PhysicsContactPostSolve in a dirty way
//  TodoSchool on Aug 24, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): PhysicsContactPostSolve is hiding valueable informations from cpArbiter, go rescue them!


#pragma once

#include <cocos/cocos2d.h>


namespace todoschool {

class PostSolveEnhancer {
    void* ContactInfo;
    
public:
    PostSolveEnhancer();
    
    bool firstContact() const;

    float restitution() const;
    float friction() const;
    float totalKineticEnergy() const;

    cocos2d::Vec2 surfaceVelocity() const;
    cocos2d::Vec2 totalImpulse() const;
    cocos2d::Vec2 totalImpulseWithFriction() const;
    
public:
    static PostSolveEnhancer fromPointer(const cocos2d::PhysicsContactPostSolve* PostSolve);
};

}  // namespace todoschool
