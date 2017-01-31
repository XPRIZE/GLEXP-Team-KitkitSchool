//
//  WoodenPuzzleStore.h -- Factory for GameBoard and PieceField
//  TodoSchool on Aug 18, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../ChildNodes/GameBoard.h"
#include "../ChildNodes/PlayField.h"
#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES

// NB(xenosoz, 2016): This interface remains only for the historical reason.
//   There were some classes named StoreFor(LatinCapital|LatinSmall|1_to_10|1_to_20).
//   Now we rely on StoreForWorksheet and exported *.tsv files instead of those ancient classes.

class WoodenPuzzleStore {
public:
    // NB(xenosoz, 2016): Create a game board with slots on it
    virtual GameBoard* createGameBoard() const = 0;

    // NB(xenosoz, 2016): Create a play field with (no/all) pieces on it
    virtual PlayField* createEmptyPlayField() const = 0;
    virtual PlayField* createFullPlayField() const = 0;
    
    virtual vector<string> motifs() const = 0;
    virtual WoodPiece* createWoodPieceWithMotif(const string& Motif) const = 0;

    // NB(xenosoz, 2016): Adjust game-playing condition for the board and field
    virtual void refinePlayCondition(GameBoard* Board, PlayField* Field) const;
};

END_NS_WOODENPUZZLES