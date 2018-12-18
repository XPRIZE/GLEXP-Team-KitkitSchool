//
//  StoreForWorksheet.h -- (Worksheet -> Store)
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "WoodenPuzzleStore.h"
#include "../PieceNodes/WoodPiece.h"
#include "../Utils/WoodenPuzzleNS.h"
#include "../Models/WoodenPuzzlesWorksheet.h"


BEGIN_NS_WOODENPUZZLES

class StoreForWorksheet: public WoodenPuzzleStore {
    typedef WoodenPuzzleStore Super;

    Worksheet TheWorksheet;
    
public:
    vector<string> slotIDs() const;
    vector<string> pieceIDs() const;
    
    GameBoard* createGameBoard() const override;
    PlayField* createEmptyPlayField() const override;
    PlayField* createFullPlayField() const override;
    
    vector<string> motifs() const override;
    WoodPiece* createWoodPieceWithMotif(const string& Motif) const override;
    void refinePlayCondition(GameBoard* Board, PlayField* Field) const override;

    string Mode;

public:
    StoreForWorksheet(const string& Mode, Worksheet WS);
    
private:
    void appendSlotsToBoard(GameBoard* Board) const;
    void appendPiecesToField(PlayField* Field) const;
};


END_NS_WOODENPUZZLES
