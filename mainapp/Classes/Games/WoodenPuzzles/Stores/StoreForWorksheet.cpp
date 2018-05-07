//
//  StoreForWorksheet.cpp on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "StoreForWorksheet.h"
#include "../ChildNodes/WoodSlot.h"
#include "../PieceNodes/WoodPiece.h"
#include "../Utils/WoodenPuzzleDepot.h"
#include "Utils/StringUtils.h"


BEGIN_NS_WOODENPUZZLES

namespace {
    string gameID(const Worksheet& WS) {
        return WS.GameID;
    }

    string contentSkin(const Worksheet& WS) {
        return WoodenPuzzleDepot().assetPrefix() + "/" + gameID(WS) + "/" + WS.BackgroundFilename;
    }
    
    string prependPrefix(const Worksheet& WS, const string& S) {
        return WoodenPuzzleDepot().assetPrefix() + "/" + gameID(WS) + "/" + S;
    }
    
    SoundEffect soundForCardinalNumber(int N) {
        return WoodenPuzzleDepot().soundForCardinalNumber(N);
    }
    
    SoundEffect soundForLetter(const string& L) {
        return WoodenPuzzleDepot().soundForLetter(L);
    }
    
    string slotIDForMotif(const Worksheet& WS, const string& Motif) {
        return gameID(WS) + "/" + toLower(Motif);  // XXX
    }

    string pieceIDForMotif(const Worksheet& WS, const string& Motif) {
        return gameID(WS) + "/" + toLower(Motif);
    }
}  // unnamed namespace


vector<string> StoreForWorksheet::slotIDs() const {
    vector<string> IDs;
    for (auto M : motifs())
        IDs.push_back(slotIDForMotif(TheWorksheet, M));
    
    return IDs;
}

vector<string> StoreForWorksheet::pieceIDs() const {
    vector<string> IDs;
    for (auto M : motifs())
        IDs.push_back(pieceIDForMotif(TheWorksheet, M));
    
    return IDs;
}

GameBoard* StoreForWorksheet::createGameBoard() const {
    auto Board = GameBoard::create();
    Board->ContentSkinPath.update(contentSkin(TheWorksheet));
    
    appendSlotsToBoard(Board);
    return Board;
}

PlayField* StoreForWorksheet::createEmptyPlayField() const {
    auto Field = PlayField::create();
    return Field;
}

PlayField* StoreForWorksheet::createFullPlayField() const {
    auto Field = PlayField::create();
    
    appendPiecesToField(Field);
    return Field;
}

vector<string> StoreForWorksheet::motifs() const {
    vector<string> Motifs;

    for (size_t I = TheWorksheet.beginProblemID(), E = TheWorksheet.endProblemID(); I < E; ++I) {
        auto Problem = TheWorksheet.problemByID(I);
        Motifs.push_back(Problem.Motif);
    }
    
    return Motifs;
}

WoodPiece* StoreForWorksheet::createWoodPieceWithMotif(const string& Motif) const {
    auto Problem = TheWorksheet.problemByMotif(Motif);
    auto It = WoodPiece::create();
    
    It->PieceID.update(pieceIDForMotif(TheWorksheet, Motif));
    It->FaceSkin.update(prependPrefix(TheWorksheet, Problem.FaceSkin));
    It->DepthSkin.update(prependPrefix(TheWorksheet, Problem.DepthSkin));
    It->ShadowSkin.update(prependPrefix(TheWorksheet, Problem.ShadowSkin));

    It->PieceNameSound = [&] {
        return (soundForLetter(Motif) ||
                soundForCardinalNumber(woodenpuzzles::stoi(Motif)));  // XXX
        
    }();
    return It;
}

StoreForWorksheet::StoreForWorksheet(Worksheet WS)
: TheWorksheet(WS)
{
}

void StoreForWorksheet::appendSlotsToBoard(GameBoard* Board) const
{
    if (!Board) { return; }
    
    Size CS = Board->getContentSize();
    
    for (size_t I = TheWorksheet.beginProblemID(), E = TheWorksheet.endProblemID(); I < E; ++I) {
        auto Problem = TheWorksheet.problemByID(I);
        string SlotID = slotIDForMotif(TheWorksheet, Problem.Motif);

        auto It = WoodSlot::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Problem.Position);

        It->SlotID.update(SlotID);
        It->FaceSkin.update(prependPrefix(TheWorksheet, Problem.FaceSkin));
        It->DepthSkin.update(prependPrefix(TheWorksheet, Problem.DepthSkin));
        It->ShadowSkin.update(prependPrefix(TheWorksheet, Problem.ShadowSkin));
        It->SlotSkin.update(prependPrefix(TheWorksheet, Problem.SlotSkin));
        Board->appendWoodSlot(It);
    }
}

void StoreForWorksheet::appendPiecesToField(PlayField* Field) const {
    if (!Field) { return; }
    
    Size CS = Field->getContentSize();
    
    WoodenPuzzleDepot Depot;
    for (auto M : motifs()) {
        auto It = createWoodPieceWithMotif(M);
        Size S = It->getContentSize();
        Rect R(S / 2.f, CS - S);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Depot.wallBiasedRandomPoint(R));
        Field->appendWoodPiece(It);
    }
}

void StoreForWorksheet::refinePlayCondition(GameBoard* Board, PlayField* Field) const {
    Super::refinePlayCondition(Board, Field);
}


END_NS_WOODENPUZZLES
