//
//  PieceBase.cpp on Aug 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "PieceBase.h"
#include "Common/Components/TargetTouchBody.h"
#include "Common/Components/TargetDragBody.h"
#include "../Utils/WoodenPuzzleDepot.h"
#include "Common/Basic/ScopeGuard.h"


BEGIN_NS_WOODENPUZZLES;


namespace {
    bool showDebugLog() { return true; }

    int localZOrderForContentSkin() { return 30; }
    int localZOrderForDepthSkin() { return 20; }
    int localZOrderForShadowSkin() { return 10; }
}  // unnamed namespace


PieceBase::PieceBase()
    : FaceNode(nullptr)
    , DepthNode(nullptr)
    , ShadowNode(nullptr)
{
}

bool PieceBase::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshComponents();
    refreshChildNodes();
    
    return true;
}

void PieceBase::clearInternals() {
    PieceID.OnValueUpdate =
    FaceSkin.OnValueUpdate =
    DepthSkin.OnValueUpdate =
    ShadowSkin.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void PieceBase::refreshComponents() {
    removeAllComponents();
}

void PieceBase::refreshChildNodes() {
    removeAllChildren();
    
    FaceNode = ([&] {
        auto It = Sprite::create(FaceSkin());
        if (!It) { return (It = nullptr); }
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(It->getContentSize() / 2.f);
        
        addChild(It, localZOrderForContentSkin());
        setContentSize(It->getContentSize());
        return It;
    }());
    
    Size CS = getContentSize();
    
    DepthNode = ([&] {
        auto It = Sprite::create(DepthSkin());
        if (!It) { return (It = nullptr); }
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        addChild(It, localZOrderForDepthSkin());
        return It;
    }());
    
    ShadowNode = ([&] {
        auto It = Sprite::create(ShadowSkin());
        if (!It) { return (It = nullptr); }
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        addChild(It, localZOrderForShadowSkin());
        return It;
    }());
}


END_NS_WOODENPUZZLES;
