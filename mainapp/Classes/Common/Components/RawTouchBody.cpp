//
//  RawTouchBody.cpp on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "RawTouchBody.h"

using namespace std;



RawTouchBody::RawTouchBody()
    : Listener(nullptr)
{
}

bool RawTouchBody::init() {
    if (!Super::init()) { return false; }
    
    setName(defaultName());
    return true;
}

string RawTouchBody::defaultName() {
    return "RawTouchBody";
}

void RawTouchBody::onEnter() {
    auto Owner = getOwner();
    if (!Owner) { return; }

    Listener = ([&] {
        auto It = EventListenerTouchOneByOne::create();
        It->setSwallowTouches(true);
        
        It->onTouchBegan = [this](Touch* T, Event* E) {
            auto Owner = getOwner();
            if (!Owner || !OnTouchDidBegin) {
                // NB(xenosoz, 2016): Catch the event even if the proper handler isn't installed (yet).
                const bool CATCH_THE_EVENT = true;
                return CATCH_THE_EVENT;
            }
            
            Owner->retain();
            bool R = false;
            for (bool B : OnTouchDidBegin(T, E))
                R = R || B;
            Owner->release();

            return R;
        };
        It->onTouchMoved = [this](Touch* T, Event* E) {
            auto Owner = getOwner();
            if (!Owner || !OnTouchDidMove) { return; }
            
            Owner->retain();
            OnTouchDidMove(T, E);
            Owner->release();
        };        
        It->onTouchEnded = [this](Touch* T, Event* E) {
            auto Owner = getOwner();
            if (!Owner || !OnTouchDidEnd) { return; }

            Owner->retain();
            OnTouchDidEnd(T, E);
            Owner->release();
        };
        It->onTouchCancelled = [this](Touch* T, Event* E) {
            auto Owner = getOwner();
            if (!Owner || !OnTouchDidCancel) { return; }
            
            Owner->retain();
            OnTouchDidCancel(T, E);
            Owner->release();
        };
        
        auto Dispatcher = Owner->getEventDispatcher();
        if (Dispatcher)
            Dispatcher->addEventListenerWithSceneGraphPriority(It, Owner);
        
        return It;
    }());
}

void RawTouchBody::onExit() {
    auto Owner = getOwner();
    if (!Owner) { return; }

    auto Dispatcher = Owner->getEventDispatcher();
    if (Dispatcher)
        Dispatcher->removeEventListener(Listener);
    
    Listener = nullptr;
}

