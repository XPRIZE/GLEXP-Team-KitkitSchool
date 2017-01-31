//
//  YellowButterfly.cpp
//  TodoSchool on Jul 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "YellowButterfly.h"
#include "../Utils/CountFieldDepot.h"


BEGIN_NS_COUNTFIELD

namespace {
    Size contentSkinSize() {
        return Size(420, 290);
    }
    
    string objectNameForSkinID(int SkinID) {
        if (SkinID == 1) { return "Yellow_Butterfly_Prime"; }
        return "Yellow_Butterfly";
    }
    
    CountFieldDepot::SpriteFrames framesForRest(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/BF_normal_%04d.png";
        const int FrameCount = 29;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
    
    CountFieldDepot::SpriteFrames framesForWalk(int SkinID) {
        CountFieldDepot Depot;
        auto ON = objectNameForSkinID(SkinID);
        string Format = Depot.assetPrefix() + "/Objects/" + ON + "/BF_out_%04d.png";
        const int FrameCount = 9;
        return Depot.framesByFileDetails(Format, FrameCount, contentSkinSize());
    }
}  // unnamed namespace


bool YellowButterfly::init(int SkinID) {
    this->SkinID = SkinID;

    if (!Super::init()) { return false; }
    return true;
}

bool YellowButterfly::init() {
    return init(0);
}

void YellowButterfly::clear() {
    Super::clear();
    
    updateWalkSprite();
    updateRestSprite();
}

Size YellowButterfly::defaultSize() {
    return contentSkinSize();
}

void YellowButterfly::updateWalkSprite() {
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

void YellowButterfly::updateRestSprite() {
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
