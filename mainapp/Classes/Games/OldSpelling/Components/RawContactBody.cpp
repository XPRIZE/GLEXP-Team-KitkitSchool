//
//  RawContactBody.cpp -- Component for listening physics contact messages
//  TodoSchool on Aug 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "RawContactBody.h"

BEGIN_NS_SPELLING;

RawContactBody::RawContactBody()
    : Listener(nullptr)
{
}

bool RawContactBody::init() {
    if (!Super::init()) { return false; }
    
    setName(defaultName());

    return true;
}

string RawContactBody::defaultName() {
    return "RawContactBody";
}

void RawContactBody::attachEventListener() {
    if (Listener) { return; }

    auto Owner = getOwner();
    if (!Owner) { return; }

    Listener = ([&] {
        auto It = EventListenerPhysicsContact::create();
        
        It->onContactBegin = [this](PhysicsContact& Contact) -> bool {
            auto Owner = getOwner();
            const bool MAKE_CONTACT = true;

            if (!Owner || !OnContactDidBegin) {
                // NB(xenosoz, 2016): Make contact even if the proper handler isn't installed (yet).
                return MAKE_CONTACT;
            }
            
            if (Contact.getShapeA()->getBody() != Owner->getPhysicsBody() &&
                Contact.getShapeB()->getBody() != Owner->getPhysicsBody()) {
                // NB(xenosoz, 2016): This event is not for this owner.
                return MAKE_CONTACT;
            }
            
            auto OwnerGuard = ScopeGuard([this] { if (getOwner()) getOwner()->retain(); },
                                         [this] { if (getOwner()) getOwner()->release(); });
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            
            bool R = false;
            for (bool B : OnContactDidBegin(&Contact))
                R = R || B;
            return R;
        };

        It->onContactPreSolve = [this](PhysicsContact& Contact, PhysicsContactPreSolve& PreSolve) -> bool {
            auto Owner = getOwner();
            const bool MAKE_CONTACT = true;

            if (!Owner || !OnContactWillSolve) { return MAKE_CONTACT; }
            
            if (Contact.getShapeA()->getBody() != Owner->getPhysicsBody() &&
                Contact.getShapeB()->getBody() != Owner->getPhysicsBody()) {
                // NB(xenosoz, 2016): This event is not for this owner.
                return MAKE_CONTACT;
            }
            
            auto OwnerGuard = ScopeGuard([this] { if (getOwner()) getOwner()->retain(); },
                                         [this] { if (getOwner()) getOwner()->release(); });
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            bool R = false;
            for (bool B : OnContactWillSolve(&Contact, &PreSolve))
                R = R || B;
            return R;
        };

        It->onContactPostSolve = [this](PhysicsContact& Contact, const PhysicsContactPostSolve& PostSolve) {
            auto Owner = getOwner();
            if (!Owner) { return; }
            
            if (Contact.getShapeA()->getBody() != Owner->getPhysicsBody() &&
                Contact.getShapeB()->getBody() != Owner->getPhysicsBody()) {
                // NB(xenosoz, 2016): This event is not for this owner.
                return;
            }

            auto OwnerGuard = ScopeGuard([this] { if (getOwner()) getOwner()->retain(); },
                                         [this] { if (getOwner()) getOwner()->release(); });
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            OnContactDidSolve(&Contact, &PostSolve);
        };

        It->onContactSeparate = [this](PhysicsContact& Contact) -> void {
            auto Owner = getOwner();
            if (!Owner) { return; }
            
            if (Contact.getShapeA()->getBody() != Owner->getPhysicsBody() &&
                Contact.getShapeB()->getBody() != Owner->getPhysicsBody()) {
                // NB(xenosoz, 2016): This event is not for this owner.
                return;
            }
            
            auto OwnerGuard = ScopeGuard([this] { if (getOwner()) getOwner()->retain(); },
                                         [this] { if (getOwner()) getOwner()->release(); });
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            
            OnContactDidEnd(&Contact);
        };
        
        auto Dispatcher = Owner->getEventDispatcher();
        if (Dispatcher)
            Dispatcher->addEventListenerWithSceneGraphPriority(It, Owner);
        
        return It;
    }());
}

void RawContactBody::detachEventListener() {
    if (Listener) { return; }

    auto Owner = getOwner();
    if (!Owner) { return; }
    
    auto Dispatcher = Owner->getEventDispatcher();
    if (Dispatcher)
        Dispatcher->removeEventListener(Listener);
    
    Listener = nullptr;
}

void RawContactBody::onEnter() {
    attachEventListener();
}

void RawContactBody::onExit() {
    detachEventListener();
}

void RawContactBody::onAdd() {
    attachEventListener();
}

void RawContactBody::onRemove() {
    detachEventListener();
}

END_NS_SPELLING;
