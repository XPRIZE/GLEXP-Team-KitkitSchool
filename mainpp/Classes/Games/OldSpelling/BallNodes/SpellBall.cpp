//
//  SpellBall.cpp -- A ball with a spell on it
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellBall.h"
#include "../Components/RawContactBody.h"
#include "../MonkeyPatches/PostSolveEnhancer.h"
#include "../Utils/SpellingDepot.h"
#include <Games/WoodenPuzzles/Components/TargetDragBody.h>


BEGIN_NS_SPELLING;

namespace {
    bool showDebugLog() { return false; }
    string contentSkin() {
        int BallID = random(0, 5);
        return SpellingDepot().assetPrefix() + format("/Temp/ball_%d.png", BallID);
    }
}  // unnamed namespace


bool SpellBall::init() {
    if (!Super::init()) { return false; }
    
    return true;
}


void SpellBall::clearInternals() {
    Super::clearInternals();
}

void SpellBall::refreshChildNodes() {
    Super::refreshChildNodes();
}

void SpellBall::refreshChildComponents() {
    Super::refreshChildComponents();

    using namespace todoschool::woodenpuzzles;  // XXX

    removeComponent(TargetDragBody::defaultName());
    removeComponent(PhysicsBody::COMPONENT_NAME);
    removeComponent(RawContactBody::defaultName());
    
    // NB(xenosoz, 2016): It is missing to set '_owner->_physicsBody = nullptr;' in PhysicsBody::onRemove.
    _physicsBody = nullptr;


    Size CS = getContentSize();
    
    [&] {
        auto It = TargetDragBody::create();

        It->OnDrag.emplace_back([this](Touch* T, Event* E) {
            auto PBody = getPhysicsBody();
            if (!PBody) { return; }
            
            Vec2 Velocity = ([&] {
                auto P = getParent();
                if (!P) { return Vec2::ZERO; }
                
                Vec2 InstantDelta = (P->convertToNodeSpace(T->getLocation()) -
                                     P->convertToNodeSpace(T->getPreviousLocation()));
                Vec2 DeltaDir = InstantDelta.getNormalized();
                float DeltaSize = InstantDelta.length();
                
                return DeltaDir * min(DeltaSize * 5, 200.f);
            }());

            // NB(xenosoz, 2016): I'm shamelessly presenting a dirty hack!
            PBody->setVelocity(Velocity);
            PBody->setAngularVelocity(0.f);
            PBody->onExit();
            
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            if (OnDrag)
                OnDrag(T, E);
        });

        It->OnDragDidEnd.emplace_back([this](Touch* T, Event* E) {
            auto PBody = getPhysicsBody();
            if (!PBody) { return; }
            
            // NB(xenosoz, 2016): I'm shamelessly presenting a dirty hack!
            PBody->onEnter();
        });
        
        addComponent(It);
    }();

    [&] {
        // NB(xenosoz, 2016): Attach a physics body
        auto It = PhysicsBody::createCircle(min(CS.width, CS.height) / 2.f,
                                            PhysicsMaterial(.1f, 0.f, .5f));
        It->setDynamic(true);
        It->setGravityEnable(true);
        It->setLinearDamping(.5f);
        
        It->setCategoryBitmask(0xFFFFFFFF);
        It->setContactTestBitmask(0xFFFFFFFF);
        It->setCollisionBitmask(0xFFFFFFFF);

        It->setVelocity(Vec2(0.f, 10.f));

        setPhysicsBody(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): Attach a contact listener
        auto It = RawContactBody::create();
        
        It->OnContactDidSolve.emplace_back([this, It](PhysicsContact* Contact, const PhysicsContactPostSolve* PostSolve) {
            auto Detail = PostSolveEnhancer::fromPointer(PostSolve);
            
            if (!Detail.firstContact() ||
                Detail.totalKineticEnergy() <= FLT_EPSILON ||
                Detail.totalImpulseWithFriction().length() <= 2e5)
            {
                // NB(xenosoz, 2016): We got a bad or small contact. Just ignore it.
                return;
            }

            if (showDebugLog()) {
                REPR(getPhysicsBody()->getVelocity().length());
                REPR(Detail.totalImpulseWithFriction());
                CCLOG("%f", Detail.totalKineticEnergy());
            }

            SpellingDepot().soundForCardDeath().play();
        });

        addComponent(It);
        return It;
    }();
}

END_NS_SPELLING;