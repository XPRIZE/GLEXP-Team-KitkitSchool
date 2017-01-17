//
//  DeckBase.cpp -- A base where new wood pieces appear.
//  TodoSchool on Aug 29, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "DeckBase.h"
#include "../Utils/WoodenPuzzleDepot.h"


BEGIN_NS_WOODENPUZZLES

namespace {
    Size contentSize() { return Size(350.f, 1735.f); }
    float durationForPieceMove() { return .2f; }
}  // unnamed namespace


bool DeckBase::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    
    return true;
}

void DeckBase::clearInternals() {
    // NB(xenosoz, 2016): Set default values
    setContentSize(contentSize());
    
    MemberPieces.clear();

    MaxPieceCount.update(3);
    MotifIndex.update(0);
}

size_t DeckBase::size() const {
    return MemberPieces.size();
}

void DeckBase::feedPuzzleStore(shared_ptr<WoodenPuzzleStore> Store) {
    ThePuzzleStore = Store;
    if (!ThePuzzleStore) { return; }
    
    Motifs = ThePuzzleStore->motifs();
}

WoodPiece* DeckBase::createNextWoodPiece() {
    if (!ThePuzzleStore) { return nullptr; }

    if (MotifIndex() >= Motifs.size()) {
        // NB(xenosoz, 2016): We meet the puzzle's end. Just return nullptr.
        return nullptr;
    }
    
    if (MemberPieces.size() >= MaxPieceCount()) {
        // NB(xenosoz, 2016): We're out of quota. Just return nullptr.
        return nullptr;
    }
    
    auto Motif = Motifs[MotifIndex()];
    auto Piece = ThePuzzleStore->createWoodPieceWithMotif(Motif);
    if (!Piece) { return nullptr; }
    
    MotifIndex.update(MotifIndex() + 1);
    return Piece;
}

void DeckBase::appendWoodPiece(WoodPiece* Piece) {
    MemberPieces.pushBack(Piece);
}

void DeckBase::removeWoodPiece(WoodPiece* Piece) {
    MemberPieces.eraseObject(Piece);
}

Point DeckBase::positionInWorldSpaceForIndex(size_t PieceIndex) const {
    Size CS = getContentSize();

    float X = CS.width / 2.f;
    float Y = CS.height * (MaxPieceCount() - PieceIndex) / (MaxPieceCount() + 1);

    Point LocalP(X, Y);
    Point WorldP = convertToWorldSpace(LocalP);

    return WorldP;
}

void DeckBase::pullPieceToHomePosition(WoodPiece* Piece, size_t PieceIndex) {
    auto Parent = Piece->getParent();
    if (!Parent) { return; }
    
    Point WorldP = positionInWorldSpaceForIndex(PieceIndex);
    Point ParentP = Parent->convertToNodeSpace(WorldP);
    
    bool ImmediateMove = false;
    if (ImmediateMove) {
        Piece->setPosition(ParentP);
    }
    else {
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(MoveTo::create(durationForPieceMove(), ParentP));
        auto Action = Sequence::create(Actions);
        Action->setTag(WoodenPuzzleDepot().actionTagForPullPiece());

        Piece->stopActionByTag(Action->getTag());
        Piece->runAction(Action);
    }
}

void DeckBase::pullPiecesToHomePosition() {
    for (size_t I = 0, E = MemberPieces.size(); I < E; ++I) {
        auto P = MemberPieces.at(I);
        pullPieceToHomePosition(P, I);
    }
}


END_NS_WOODENPUZZLES
