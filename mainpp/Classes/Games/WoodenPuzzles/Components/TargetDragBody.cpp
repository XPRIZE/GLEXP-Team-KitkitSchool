//
//  TargetDragBody.cpp on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TargetDragBody.h"
#include "TargetTouchBody.h"


BEGIN_NS_WOODENPUZZLES;

TargetDragBody::TargetDragBody() {
}

bool TargetDragBody::init() {
    if (!Super::init()) { return false; }
    
    setName(defaultName());
    return true;
}

string TargetDragBody::defaultName() {
    return "TargetDragBody";
}

void TargetDragBody::onAdd() {
    Super::onAdd();
    
    auto Owner = getOwner();
    if (!Owner) { return; }
    
    [&] {
        if (!Owner->getComponent(TargetTouchBody::defaultName()))
            Owner->addComponent(TargetTouchBody::create());
        
        auto It = dynamic_cast<TargetTouchBody*>(Owner->getComponent(TargetTouchBody::defaultName()));
        if (!It) { return (It = nullptr); }
        
        It->OnTouchDidBegin.emplace_back([this](Touch* T, Event* E) -> bool {
            auto Owner = getOwner();
            if (!Owner) { return false; }
            
            bool R = true;
            if (OnDragDidBegin) {
                Owner->retain();
                R = false;
                for (bool B : OnDragDidBegin(T, E))
                    R = R || B;
                Owner->release();
            }

            return R;
        });
        
        It->OnTouchDidMove.emplace_back([this](Touch* T, Event* E) {
            auto Owner = getOwner();
            if (!Owner) { return; }
            
            auto P = Owner->getParent();
            if (!P) { return; }
            
            Vec2 InstantDelta = (P->convertToNodeSpace(T->getLocation()) -
                                 P->convertToNodeSpace(T->getPreviousLocation()));
            Vec2 Delta = (P->convertToNodeSpace(T->getLocation()) -
                          P->convertToNodeSpace(T->getStartLocation()));
            
            // NB(xenosoz, 2016): Move along the drag.
            Owner->setPosition(Owner->getPosition() + InstantDelta);
            
            if (OnDrag) {
                Owner->retain();
                OnDrag(T, E);
                Owner->release();
            }
        });
        
        It->OnTouchDidEnd.emplace_back([this](Touch* T, Event* E) {
            auto Owner = getOwner();
            if (!Owner) { return; }

            if (OnDragDidEnd) {
                Owner->retain();
                OnDragDidEnd(T, E);
                Owner->release();
            }
        });

        return It;
    }();
}

void TargetDragBody::onRemove() {
    Super::onRemove();
    
    auto Owner = getOwner();
    if (!Owner) { return; }

    // XXX: Uninstall callback handlers here.
}

END_NS_WOODENPUZZLES;