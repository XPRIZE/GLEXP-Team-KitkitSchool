//
//  WoodPieceShadow.cpp -- Shadow of WoodPiece
//  TodoSchool on Aug 19, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodPieceShadow.h"
#include "../Utils/WoodenPuzzleDepot.h"


BEGIN_NS_WOODENPUZZLES

namespace {
    string shadowSkin(const string& PieceID) {
        return WoodenPuzzleDepot().assetPrefix() + "/" + format("%s_3.png", PieceID.c_str());
    }
}  // unnamed namespace


bool WoodPieceShadow::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}


void WoodPieceShadow::clearInternals() {
    PieceID.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
    PositionInWorldSpace.OnValueUpdate = [this](Point&) {
        auto Parent = getParent();
        if (Parent)
            setPosition(Parent->convertToNodeSpace(PositionInWorldSpace()));
    };
}

void WoodPieceShadow::refreshChildNodes() {
    removeAllChildren();
    
    [&] {
        auto It = Sprite::create(shadowSkin(PieceID()));
        if (!It) {
            setContentSize(Size::ZERO);
            return (It = nullptr);
        }
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(It->getContentSize() / 2.f);
        
        addChild(It);
        setContentSize(It->getContentSize());
        return It;
    }();
}


END_NS_WOODENPUZZLES
