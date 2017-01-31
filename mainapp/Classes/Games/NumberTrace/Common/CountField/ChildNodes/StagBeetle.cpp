//
//  StagBeetle.cpp
//  TodoSchool on Jun 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "StagBeetle.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    Size contentSkinSize() {
        return Size(440, 470);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Stag_Beetle_Prime"; }
        return "Stag_Beetle";
    }
    
    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/SB_normal_%04d.png";
        const int FrameCount = 36;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/SB_out_%04d.png";
        const int FrameCount = 13;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool StagBeetle::init(int SkinID) {
    this->SkinID = SkinID;
    
    if (!Super::init()) { return false; }
    return true;
}

bool StagBeetle::init() {
    return init(0);
}

void StagBeetle::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size StagBeetle::defaultSize() {
    return contentSkinSize();
}

void StagBeetle::updateWalkSprite() {
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

void StagBeetle::updateRestSprite() {
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

}  // namespace countfield
}  // namespace todoschool
