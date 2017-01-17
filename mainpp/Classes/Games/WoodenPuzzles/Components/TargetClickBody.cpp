//
//  TargetClickBody.cpp on Aug 16, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TargetClickBody.h"
#include "TargetTouchBody.h"


BEGIN_NS_WOODENPUZZLES;

TargetClickBody::TargetClickBody() {
}

bool TargetClickBody::init() {
    if (!Super::init()) { return false; }
    
    setName(defaultName());
    return true;
}

string TargetClickBody::defaultName() {
    return "TargetClickBody";
}

void TargetClickBody::onAdd() {
    Super::onAdd();
    
    auto Owner = getOwner();
    if (!Owner) { return; }
    
    [&] {
        if (!Owner->getComponent(TargetTouchBody::defaultName()))
            Owner->addComponent(TargetTouchBody::create());
        
        auto It = dynamic_cast<TargetTouchBody*>(Owner->getComponent(TargetTouchBody::defaultName()));
        if (!It) { return (It = nullptr); }
        
        using namespace std::placeholders;
        It->OnTouchDidBegin.emplace_back(bind(&ThisType::handleTouchDidBegin, this, _1, _2));
        It->OnTouchDidMove.emplace_back(bind(&ThisType::handleTouchDidMove, this, _1, _2));
        It->OnTouchDidEnd.emplace_back(bind(&ThisType::handleTouchDidEnd, this, _1, _2));
        It->OnTouchDidCancel.emplace_back(bind(&ThisType::handleTouchDidCancel, this, _1, _2));
        
        return It;
    }();
}

void TargetClickBody::onRemove() {
    Super::onRemove();
    
    auto Owner = getOwner();
    if (!Owner) { return; }

    // XXX: Uninstall callback handlers here.
}

bool TargetClickBody::handleTouchDidBegin(Touch* T, Event* E) {
    return true;
}

void TargetClickBody::handleTouchDidMove(Touch* T, Event* E) {
    // NB(xenosoz, 2016): Do nothing.
}

void TargetClickBody::handleTouchDidEnd(Touch* T, Event* E) {
    auto Owner = getOwner();
    if (!Owner) { return; }

    if (OnClick) {
        Owner->retain();
        OnClick(T, E);
        Owner->release();
    }
}

void TargetClickBody::handleTouchDidCancel(Touch* T, Event* E) {
    // NB(xenosoz, 2016): Do nothing.
}


END_NS_WOODENPUZZLES;