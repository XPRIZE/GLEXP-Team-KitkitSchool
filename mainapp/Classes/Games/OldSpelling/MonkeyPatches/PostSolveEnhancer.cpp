//
//  PostSolveEnhancer.cpp -- Extract more information from PhysicsContactPostSolve in a dirty way
//  TodoSchool on Aug 24, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "PostSolveEnhancer.h"
#include <cocos/physics/CCPhysicsHelper.h>
#include <chipmunk/chipmunk.h>


namespace todoschool {
    
PostSolveEnhancer::PostSolveEnhancer()
    : ContactInfo(nullptr)
{
}

bool PostSolveEnhancer::firstContact() const {
    if (!ContactInfo) { return false; }
    auto A = static_cast<cpArbiter*>(ContactInfo);
    
    return cpArbiterIsFirstContact(A);
}

float PostSolveEnhancer::restitution() const {
    if (!ContactInfo) { return 0.f; }
    auto A = static_cast<cpArbiter*>(ContactInfo);
    
    return cpArbiterGetRestitution(A);
}

float PostSolveEnhancer::friction() const {
    if (!ContactInfo) { return 0.f; }
    auto A = static_cast<cpArbiter*>(ContactInfo);

    return cpArbiterGetFriction(A);
}
    
float PostSolveEnhancer::totalKineticEnergy() const {
    if (!ContactInfo) { return 0.f; }
    auto A = static_cast<cpArbiter*>(ContactInfo);
    
    return cpArbiterTotalKE(A);
}

cocos2d::Vec2 PostSolveEnhancer::surfaceVelocity() const {
    if (!ContactInfo) { return cocos2d::Vec2::ZERO; }
    auto A = static_cast<cpArbiter*>(ContactInfo);

    cpVect SV = cpArbiterGetSurfaceVelocity(A);
    return cocos2d::PhysicsHelper::cpv2point(SV);
}
    
cocos2d::Vec2 PostSolveEnhancer::totalImpulse() const {
    if (!ContactInfo) { return cocos2d::Vec2::ZERO; }
    auto A = static_cast<cpArbiter*>(ContactInfo);
    
    cpVect SV = cpArbiterTotalImpulse(A);
    return cocos2d::PhysicsHelper::cpv2point(SV);
}
    
cocos2d::Vec2 PostSolveEnhancer::totalImpulseWithFriction() const {
    if (!ContactInfo) { return cocos2d::Vec2::ZERO; }
    auto A = static_cast<cpArbiter*>(ContactInfo);
    
    cpVect SV = cpArbiterTotalImpulse(A);
    return cocos2d::PhysicsHelper::cpv2point(SV);
}
    
    
PostSolveEnhancer PostSolveEnhancer::fromPointer(const cocos2d::PhysicsContactPostSolve* PostSolve) {
    PostSolveEnhancer Enhancer;

    // NB(xenosoz, 2016): Dirty hack coming through.
    if (sizeof(PostSolveEnhancer) != sizeof(cocos2d::PhysicsContactPostSolve)) { return Enhancer; }
    
    auto That = reinterpret_cast<const PostSolveEnhancer*>(PostSolve);
    if (!That) { return Enhancer; }
    
    Enhancer.ContactInfo = That->ContactInfo;
    return Enhancer;
}

}  // namespace todoschool
