//
//  DeckBase.h -- A base where new wood pieces appear.
//  TodoSchool on Aug 29, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../PieceNodes/WoodPiece.h"
#include "../Stores/WoodenPuzzleStore.h"
#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES

class DeckBase: public Node {
    typedef Node Super;

    shared_ptr<WoodenPuzzleStore> ThePuzzleStore;
    vector<string> Motifs;
    
    // NB(xenosoz, 2016): 
    Vector<WoodPiece*> MemberPieces;


public:
    Chain<string> Mode;
    Chain<size_t> MaxPieceCount;
    Chain<size_t> MotifIndex;

public:
    CREATE_FUNC(DeckBase);
    bool init() override;
    
    size_t size() const;
    void feedPuzzleStore(shared_ptr<WoodenPuzzleStore> Store);
    
    // NB(xenosoz, 2016): Create next wood piece whenever possible. Return nullptr when we hit the end of puzzles.
    WoodPiece* createNextWoodPiece();

    void appendWoodPiece(WoodPiece* Piece);
    void removeWoodPiece(WoodPiece* Piece);

    Point positionInWorldSpaceForIndex(size_t PieceIndex) const;
    void pullPieceToHomePosition(WoodPiece* Piece, size_t PieceIndex);
    void pullPiecesToHomePosition();


private:
    void clearInternals();
};

END_NS_WOODENPUZZLES
