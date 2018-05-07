//
//  Cockroach.cpp on Jul 15, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Cockroach.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    float hotfixScale() { return 2.f / 3.f; }

    Size contentSkinSize() {
        return Size(678, 784);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Cockroach_Prime"; }
        return "Cockroach";
    }
    
    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/cockroach_normal_%04d.png";
        const int FrameCount = 37;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/cockroach_out_%04d.png";
        const int FrameCount = 9;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool Cockroach::init(int SkinID) {
    this->SkinID = SkinID;
    if (!Super::init()) { return false; }

    return true;
}

bool Cockroach::init() {
    return init(0);
}

void Cockroach::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size Cockroach::defaultSize() {
    return contentSkinSize() * hotfixScale();
}

void Cockroach::updateWalkSprite() {
    CountFieldDepot Depot;
    CountFieldDepot::SpriteFrames WalkFrames = framesForWalk(SkinID);
    
    if (WalkSprite)
        WalkSprite->stopAction(WalkAction);
    
    WalkAction = Depot.createLoopActionForSpriteFrames(WalkFrames);
    WalkSprite = ([&] {
        Sprite* It = Depot.createSpriteForSpriteFrames(WalkFrames);
        It->runAction(WalkAction);
        
        It->setScale(hotfixScale());
        It->setVisible(Moving());
        addChild(It);
        
        return It;
    }());
    
    framesForWalk(SkinID);
}

void Cockroach::updateRestSprite() {
    CountFieldDepot Depot;
    CountFieldDepot::SpriteFrames RestFrames = framesForRest(SkinID);
    
    if (RestSprite)
        RestSprite->stopAction(RestAction);
    
    RestAction = Depot.createLoopActionForSpriteFrames(RestFrames);
    RestSprite = ([&] {
        Sprite* It = Depot.createSpriteForSpriteFrames(RestFrames);
        It->runAction(RestAction);
        
        It->setScale(hotfixScale());
        It->setVisible(!Moving());
        addChild(It);
        
        return It;
    }());
}

END_NS_COUNTFIELD

