//
//  PlayField.cpp -- A field where unfinished wood pieces remain.
//  TodoSchool on Aug 17, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "PlayField.h"
#include "../Utils/WoodenPuzzleDepot.h"

BEGIN_NS_WOODENPUZZLES;


namespace {
    Size contentSize(const string& Mode) { return WoodenPuzzleDepot(Mode).gameSize(); }
}  // unnamed namespace


bool PlayField::init(const string& Mode) {
    if (!Super::init()) { return false; }
    
    this->Mode = Mode;
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void PlayField::appendWoodPiece(WoodPiece* Piece) {
    addChild(Piece);
    WoodPieces.insert({Piece->PieceID(), Piece});
}

void PlayField::removeWoodPiece(WoodPiece* Piece) {
    removeChild(Piece);
    WoodPieces.erase(Piece->PieceID());
}

void PlayField::renice(WoodPiece* Piece) {
    // NB(xenosoz):
    //   Case #1 (RankCount == 1): 0 1 [2] 3 4 -> 0 1 {4} 2 3
    //   Case #2 (RankCount >= 2): 0 0 0 1 1 [1] 2 2 2 -> 0 0 0 1 1 {3} 2 2 2
    
    int PivotZ = Piece->getLocalZOrder();
    int MaxZ = numeric_limits<int>::min();
    
    int RankCount = 0;
    for (auto Item : WoodPieces) {
        WoodPiece* P = Item.second;
        if (!P) { continue; }
        
        if (P->getLocalZOrder() == PivotZ)
            RankCount += 1;
        MaxZ = max(MaxZ, P->getLocalZOrder());
    }
    
    if (RankCount > 1) {
        // NB(xenosoz, 2016): There's many pieces with same local-z. Promote this piece only.
        Piece->setLocalZOrder(MaxZ + 1);
        return;
    }
    
    // NB(xenosoz, 2016): There's only one (this piece) with same local-z. Collapse this local-z.
    for (auto Item : WoodPieces) {
        WoodPiece* P = Item.second;
        if (!P) { continue; }

        if (P->getLocalZOrder() > PivotZ)
            P->setLocalZOrder(P->getLocalZOrder() - 1);
    }

    // NB(xenosoz, 2016): Now there's no piece with MaxZ. Promote this piece to MaxZ.
    Piece->setLocalZOrder(MaxZ);
}

void PlayField::clearInternals() {
    setContentSize(contentSize(Mode));
}

void PlayField::refreshChildNodes() {
}


END_NS_WOODENPUZZLES;
