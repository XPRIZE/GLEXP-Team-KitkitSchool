//
//  PlayField.h -- A field where unfinished wood pieces remain.
//  TodoSchool on Aug 17, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../PieceNodes/WoodPiece.h"
#include "../Utils/WoodenPuzzleNS.h"
#include <Common/Basic/CreateFunc.h>


BEGIN_NS_WOODENPUZZLES;


class PlayField: public Node {
    typedef Node Super;

public:
    map<string, WoodPiece*> WoodPieces;

public:
    CREATE_FUNC(PlayField);
    bool init(const string& Mode);
    
    string Mode;    
    void appendWoodPiece(WoodPiece* Piece);
    void removeWoodPiece(WoodPiece* Piece);
    
    // NB(xenosoz, 2016): Adjust to highest local z order among WoodPieces.
    void renice(WoodPiece* Piece);

private:
    void clearInternals();
    void refreshChildNodes();
};


END_NS_WOODENPUZZLES;
