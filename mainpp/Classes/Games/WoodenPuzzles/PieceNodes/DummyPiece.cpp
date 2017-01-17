//
//  DummyPiece.cpp on Aug 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "DummyPiece.h"


BEGIN_NS_WOODENPUZZLES

bool DummyPiece::init() {
    if (!Super::init()) { return false; }
    
    return true;
}

void DummyPiece::switchTo2D() {
    if (FaceNode) { FaceNode->setVisible(true); }
    if (DepthNode) { DepthNode->setVisible(false); }
    if (ShadowNode) { ShadowNode->setVisible(false); }
}

void DummyPiece::switchTo3D() {
    if (FaceNode) { FaceNode->setVisible(true); }
    if (DepthNode) { DepthNode->setVisible(true); }
    if (ShadowNode) { ShadowNode->setVisible(true); }
}

void DummyPiece::clearInternals() {
    Super::clearInternals();
}

void DummyPiece::refreshComponents() {
    Super::refreshComponents();
}

void DummyPiece::refreshChildNodes() {
    Super::refreshChildNodes();
}


END_NS_WOODENPUZZLES
