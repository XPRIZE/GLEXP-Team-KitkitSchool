//
//  ShadowField.h on Aug 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "WoodPieceShadow.h"
#include "../PieceNodes/WoodPiece.h"
#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES

class ShadowField: public Node {
    typedef Node Super;
    
    unordered_map<WoodPiece*, WoodPieceShadow*> ShadowMap;

public:
    CREATE_FUNC(ShadowField);
    bool init() override;
    
    void appendShadowForPiece(WoodPieceShadow* S, WoodPiece* P);
    void removeShadowForPiece(WoodPiece* P);
    
public:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_WOODENPUZZLES
