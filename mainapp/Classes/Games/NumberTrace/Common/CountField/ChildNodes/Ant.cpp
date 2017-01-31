//
//  Ant.cpp
//  TodoSchool on Jun 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Ant.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    float hotfixScale() { return 3.f / 4.f; }

    Size contentSkinSize() {
        return Size(460, 570);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Ant_Prime"; }
        return "Ant";
    }

    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        // NB(xenosoz, 2016): It's sprite #32 in `ant.swf'.
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/ant_normal_%04d.png";
        const int FrameCount = 32;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        // NB(xenosoz, 2016): It's sprite #31 in `ant.swf'.
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/ant_out_%04d.png";
        const int FrameCount = 13;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool Ant::init(int SkinID) {
    this->SkinID = SkinID;
    
    if (!Super::init()) { return false; }
    return true;
}

bool Ant::init() {
    return init(0);
}

void Ant::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size Ant::defaultSize() {
    return contentSkinSize() * hotfixScale();
}

void Ant::updateWalkSprite() {
    CountFieldDepot Depot;
    auto WalkFrames = framesForWalk(SkinID);
    
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
    
    framesForRest(SkinID);
}

void Ant::updateRestSprite() {
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
