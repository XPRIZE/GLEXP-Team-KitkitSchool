//
//  Spider.cpp
//  TodoSchool on Jun 29, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "Spider.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    Size contentSkinSize() {
        return Size(440, 485);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Spider_Prime"; }
        return "Spider";
    }
    
    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/spider_normal_%04d.png";
        const int FrameCount = 16;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/spider_out_%04d.png";
        const int FrameCount = 13;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool Spider::init(int SkinID) {
    this->SkinID = SkinID;
    
    if (!Super::init()) { return false; }
    return true;
}

bool Spider::init() {
    return init(0);
}

void Spider::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size Spider::defaultSize() {
    return contentSkinSize();
}

void Spider::updateWalkSprite() {
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

void Spider::updateRestSprite() {
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
