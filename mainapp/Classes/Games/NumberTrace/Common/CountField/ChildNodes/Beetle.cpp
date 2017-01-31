//
//  Beetle.cpp
//  TodoSchool on Jun 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "Beetle.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    Size contentSkinSize() {
        return Size(430, 442);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Beetle_Prime"; }
        return "Beetle";
    }
    
    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/beetle_normal_%04d.png";
        const int FrameCount = 35;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/beetle_out_%04d.png";
        const int FrameCount = 16;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool Beetle::init(int SkinID) {
    this->SkinID = SkinID;
    
    if (!Super::init()) { return false; }
    return true;
}

bool Beetle::init() {
    return init(0);
}

void Beetle::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size Beetle::defaultSize() {
    return contentSkinSize();
}

void Beetle::updateWalkSprite() {
    CountFieldDepot Depot;
    CountFieldDepot::SpriteFrames WalkFrames = framesForWalk(SkinID);
    
    if (WalkSprite)
        WalkSprite->stopAction(WalkAction);
    
    WalkAction = Depot.createLoopActionForSpriteFrames(WalkFrames);
    WalkSprite = ([&] {
        Sprite* It = Depot.createSpriteForSpriteFrames(WalkFrames);
        It->runAction(WalkAction);
        
        It->setVisible(Moving());
        addChild(It);
        
        return It;
    }());
    
    framesForRest(SkinID);
}

void Beetle::updateRestSprite() {
    CountFieldDepot Depot;
    CountFieldDepot::SpriteFrames RestFrames = framesForRest(SkinID);
    
    if (RestSprite)
        RestSprite->stopAction(RestAction);
    
    RestAction = Depot.createLoopActionForSpriteFrames(RestFrames);
    RestSprite = ([&] {
        Sprite* It = Depot.createSpriteForSpriteFrames(RestFrames);
        It->runAction(RestAction);
        
        It->setVisible(!Moving());
        addChild(It);
        
        return It;
    }());
}

END_NS_COUNTFIELD
