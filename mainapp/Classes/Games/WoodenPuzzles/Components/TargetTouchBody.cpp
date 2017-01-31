//
//  TargetTouchBody.cpp on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TargetTouchBody.h"
#include "RawTouchBody.h"
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>


BEGIN_NS_WOODENPUZZLES

TargetTouchBody::TargetTouchBody()
: CachedHitCamera(nullptr)
{
}

bool TargetTouchBody::init() {
    if (!Super::init()) { return false; }
    
    setName(defaultName());
    return true;
}

string TargetTouchBody::defaultName() {
    return "TargetTouchBody";
}

void TargetTouchBody::onAdd() {
    Super::onAdd();
    
    auto Owner = getOwner();
    if (!Owner) { return; }

    [&] {
        if (!Owner->getComponent(RawTouchBody::defaultName()))
            Owner->addComponent(RawTouchBody::create());
        
        auto It = dynamic_cast<RawTouchBody*>(Owner->getComponent(RawTouchBody::defaultName()));
        if (!It) { return (It = nullptr); }

        using namespace std::placeholders;
        It->OnTouchDidBegin.emplace_back(bind(&ThisType::handleTouchDidBegin, this, _1, _2));
        It->OnTouchDidMove.emplace_back(bind(&ThisType::handleTouchDidMove, this, _1, _2));
        It->OnTouchDidEnd.emplace_back(bind(&ThisType::handleTouchDidEnd, this, _1, _2));
        It->OnTouchDidCancel.emplace_back(bind(&ThisType::handleTouchDidCancel, this, _1, _2));

        return It;
    }();
}

void TargetTouchBody::onRemove() {
    Super::onRemove();

    auto Owner = getOwner();
    if (!Owner) { return; }

    // XXX: Uninstall callback handlers here.
}

bool TargetTouchBody::handleTouchDidBegin(Touch* T, Event* E) {
    auto Owner = getOwner();
    if (!Owner || !Owner->isVisible()) { return false; }
    
    bool Hit = false;
    
    Point TouchPoint = T->getLocation();
    auto VC = Camera::getVisitingCamera();
    if (containsPointInContentRectWithCamera(TouchPoint, VC)) {
        if (!clippedOutByAncestors(TouchPoint)) {
            CachedHitCamera = VC;
            Hit = true;
        }
    }
    if (!Hit) { return false; }
    
    Highlighted.update(true);

    bool R = true;
    if (OnTouchDidBegin) {
        auto Guard = NodeScopeGuard(Owner);

        R = false;
        for (bool B : OnTouchDidBegin(T, E))
            R = R || B;
    }
    
    return R;
}

void TargetTouchBody::handleTouchDidMove(Touch* T, Event* E) {
    auto Owner = getOwner();
    if (!Owner) { return; }
    
    Point TouchPoint = T->getLocation();

    Highlighted.update(!!containsPointInContentRectWithCamera(TouchPoint, CachedHitCamera));

    if (OnTouchDidMove) {
        auto Guard = NodeScopeGuard(Owner);
        OnTouchDidMove(T, E);
    }
}

void TargetTouchBody::handleTouchDidEnd(Touch* T, Event* E) {
    auto Owner = getOwner();
    if (!Owner) { return; }
    
    Highlighted.update(false);
    
    if (OnTouchDidEnd) {
        auto Guard = NodeScopeGuard(Owner);

        OnTouchDidEnd(T, E);
    }
}

void TargetTouchBody::handleTouchDidCancel(Touch* T, Event* E) {
    auto Owner = getOwner();
    if (!Owner) { return; }
    
    Highlighted.update(false);

    if (OnTouchDidCancel) {
        auto Guard = NodeScopeGuard(Owner);

        OnTouchDidCancel(T, E);
    }
}

Optional<Vec3>
TargetTouchBody::containsPointInContentRectWithCamera(const Vec2& P, const Camera* C) const
{
    auto Owner = getOwner();
    if (!Owner) { return Optional<Vec3>(); }
    
    Rect R(Point::ZERO, Owner->getContentSize());
    Vec3 RetHitPoint;
    
    bool Hit = isScreenPointInRect(P, C,
                                   Owner->getWorldToNodeTransform(), R,
                                   &RetHitPoint);
    
    return (Hit ? Optional<Vec3>(RetHitPoint) : Optional<Vec3>());
}

bool TargetTouchBody::clippedOutByAncestors(Point P) {
    auto Owner = getOwner();
    if (!Owner) { return true; }

    for (auto Parent = Owner->getParent(); Parent; Parent = Parent->getParent())
    {
        // NB(xenosoz, 2016): Find clipping parent clipping parents.
        Layout* L = dynamic_cast<Layout*>(Parent);
        if (!L) { continue; }
        if (!L->isClippingEnabled()) { continue; }
        
        auto VC = Camera::getVisitingCamera();
        VC = (VC ? VC : CachedHitCamera);
        
        if (!L->hitTest(P, VC, nullptr)) {
            // NB(xenosoz, 2016): Point P is cut by an ancestor L.
            return true;
        }
    }
    
    return false;
}

END_NS_WOODENPUZZLES
