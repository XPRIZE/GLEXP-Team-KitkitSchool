//
//  ShadowField.cpp on Aug 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ShadowField.h"


BEGIN_NS_WOODENPUZZLES

bool ShadowField::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void ShadowField::appendShadowForPiece(WoodPieceShadow* S, WoodPiece* P) {
    addChild(S);
    ShadowMap.insert({P, S});
}

void ShadowField::removeShadowForPiece(WoodPiece* P) {
    auto I = ShadowMap.find(P);
    if (I != ShadowMap.end()) {
        auto S = I->second;
        if (S)
            removeChild(S);
    }

    ShadowMap.erase(P);
}

void ShadowField::clearInternals() {
    
}

void ShadowField::refreshChildNodes() {
    
}

END_NS_WOODENPUZZLES