//
//  Moth.cpp
//  TodoSchool on Jul 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Moth.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    Size contentSkinSize() {
        return Size(400, 289);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Moth_Prime"; }
        return "Moth";
    }
    
    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/moth_normal_%04d.png";
        const int FrameCount = 23;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/moth_out_%04d.png";
        const int FrameCount = 10;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool Moth::init(int SkinID) {
    this->SkinID = SkinID;
    
    if (!Super::init()) { return false; }
    return true;
}

bool Moth::init() {
    return init(0);
}

void Moth::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size Moth::defaultSize() {
    return contentSkinSize();
}

void Moth::updateWalkSprite() {
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

void Moth::updateRestSprite() {
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
