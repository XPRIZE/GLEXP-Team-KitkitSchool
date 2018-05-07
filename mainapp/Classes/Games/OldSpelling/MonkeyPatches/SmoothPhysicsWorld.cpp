//
//  SmoothPhysicsWorld.cpp -- Physics world with smoothed deltaTime
//  TodoSchool on Aug 24, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SmoothPhysicsWorld.h"

#include "chipmunk/chipmunk.h"
#include <cocos/physics/CCPhysicsBody.h>
#include <cocos/physics/CCPhysicsShape.h>
#include <cocos/physics/CCPhysicsContact.h>
#include <cocos/physics/CCPhysicsJoint.h>
#include <cocos/physics/CCPhysicsContact.h>
#include <cocos/physics/CCPhysicsHelper.h>


namespace todoschool {
    
SmoothPhysicsWorld::SmoothPhysicsWorld()
    : Super()
    , _cachedUpdateDeltaTime(0.f)
{
}

void SmoothPhysicsWorld::update(float delta, bool userCall) {
    //Super::update(delta, userCall);
    
    if(!_delayAddBodies.empty())
        updateBodies();
    else if (!_delayRemoveBodies.empty())
        updateBodies();
    
    auto sceneToWorldTransform = _scene->getNodeToParentTransform();
    beforeSimulation(_scene, sceneToWorldTransform, 1.f, 1.f, 0.f);
    
    if (!_delayAddJoints.empty() || !_delayRemoveJoints.empty())
        updateJoints();
    
    if (delta < FLT_EPSILON)
        return;
    
    if (userCall) {
        cpSpaceStep(_cpSpace, delta);
    }
    else {
        _updateTime += delta;
        if (++_updateRateCount >= _updateRate) {
            // NB(xenosoz, 2016): Credit to @Heyalda
            //   http://discuss.cocos2d-x.org/t/why-jitter-jerk-when-following-sprite-with-layer-video-included/16799/15

            const float dt = _updateTime * _speed / _substeps;
            const float alpha = 0.85f;
            const float filteredDeltaTime = (dt > __FLT_EPSILON__ ?
                                             _cachedUpdateDeltaTime * (1.f - alpha) + dt * alpha :
                                             0.f);

            for (int i = 0; i < _substeps; ++i)
                cpSpaceStep(_cpSpace, filteredDeltaTime);

            _updateRateCount = 0;
            _updateTime = 0.0f;
            _cachedUpdateDeltaTime = filteredDeltaTime;
        }
    }

    if (_debugDrawMask != DEBUGDRAW_NONE)
        debugDraw();
    
    // Update physics position, should loop as the same sequence as node tree.
    // PhysicsWorld::afterSimulation() will depend on the sequence.
    afterSimulation(_scene, sceneToWorldTransform, 0.f);
}
    
SmoothPhysicsWorld* SmoothPhysicsWorld::construct(cocos2d::Scene* scene) {
    auto world = new(std::nothrow) SmoothPhysicsWorld();
    if (world && world->init()) {
        world->_scene = scene;
        world->_eventDispatcher = scene->getEventDispatcher();
        return world;
    }

    CC_SAFE_DELETE(world);
    return nullptr;
}

}  // namespace todoschool
