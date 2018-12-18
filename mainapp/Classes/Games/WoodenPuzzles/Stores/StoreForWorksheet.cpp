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
#include "../Utils/WoodenPuzzleUtil.h"
#include "Utils/StringUtils.h"


BEGIN_NS_WOODENPUZZLES

namespace {
    string gameID(const Worksheet& WS) {
        return WS.GameID;
    }

    string contentSkin(const string& Mode, const Worksheet& WS) {
        return WoodenPuzzleDepot(Mode).assetPrefix() + "/" + gameID(WS) + "/" + WS.BackgroundFilename;
    }
    
    string prependPrefix(const string& Mode, const Worksheet& WS, const string& S) {
        return WoodenPuzzleDepot(Mode).assetPrefix() + "/" + gameID(WS) + "/" + S;
    }
    
    SoundEffect soundForCardinalNumber(const string& Mode, int N) {
        return WoodenPuzzleDepot(Mode).soundForCardinalNumber(N);
    }
    
    SoundEffect soundForLetter(const string& Mode, const string& L) {
        return WoodenPuzzleDepot(Mode).soundForLetter(L);
    }
    
    string slotIDForMotif(const string& Mode, const Worksheet& WS, const string& Motif) {
        return gameID(WS) + "/" + toLower(Motif);  // XXX
    }

    string pieceIDForMotif(const string& Mode, const Worksheet& WS, const string& Motif) {
        return gameID(WS) + "/" + toLower(Motif);
    }
}  // unnamed namespace


vector<string> StoreForWorksheet::slotIDs() const {
    vector<string> IDs;
    for (auto M : motifs())
        IDs.push_back(slotIDForMotif(Mode, TheWorksheet, M));
    
    return IDs;
}

vector<string> StoreForWorksheet::pieceIDs() const {
    vector<string> IDs;
    for (auto M : motifs())
        IDs.push_back(pieceIDForMotif(Mode, TheWorksheet, M));
    
    return IDs;
}

GameBoard* StoreForWorksheet::createGameBoard() const {
    auto Board = GameBoard::create();
    Board->ContentSkinPath.update(contentSkin(Mode, TheWorksheet));
    
    appendSlotsToBoard(Board);
    return Board;
}

PlayField* StoreForWorksheet::createEmptyPlayField() const {
    auto Field = PlayField::create(Mode);
    return Field;
}

PlayField* StoreForWorksheet::createFullPlayField() const {
    auto Field = PlayField::create(Mode);
    
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
    
    It->PieceID.update(pieceIDForMotif(Mode, TheWorksheet, Motif));
    It->FaceSkin.update(prependPrefix(Mode, TheWorksheet, Problem.FaceSkin));
    It->DepthSkin.update(prependPrefix(Mode, TheWorksheet, Problem.DepthSkin));
    It->ShadowSkin.update(prependPrefix(Mode, TheWorksheet, Problem.ShadowSkin));

    It->PieceNameSound = [&] {
        return (soundForLetter(Mode, Motif) ||
                soundForCardinalNumber(Mode, woodenpuzzles::stoi(Motif)));  // XXX
        
    }();
    return It;
}

StoreForWorksheet::StoreForWorksheet(const string& Mode, Worksheet WS)
: Mode(Mode)
, TheWorksheet(WS)
{
}

void StoreForWorksheet::appendSlotsToBoard(GameBoard* Board) const
{
    if (!Board) { return; }
    
    Size CS = Board->getContentSize();
    
    for (size_t I = TheWorksheet.beginProblemID(), E = TheWorksheet.endProblemID(); I < E; ++I) {
        auto Problem = TheWorksheet.problemByID(I);
        string SlotID = slotIDForMotif(Mode, TheWorksheet, Problem.Motif);

        auto It = WoodSlot::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Problem.Position);

        It->Mode.update(Mode);
        It->SlotID.update(SlotID);
        It->FaceSkin.update(prependPrefix(Mode, TheWorksheet, Problem.FaceSkin));
        It->DepthSkin.update(prependPrefix(Mode, TheWorksheet, Problem.DepthSkin));
        It->ShadowSkin.update(prependPrefix(Mode, TheWorksheet, Problem.ShadowSkin));
        It->SlotSkin.update(prependPrefix(Mode, TheWorksheet, Problem.SlotSkin));
        Board->appendWoodSlot(It);
    }
}

void StoreForWorksheet::appendPiecesToField(PlayField* Field) const {
    if (!Field) { return; }
    
    Size CS = Field->getContentSize();
    
    WoodenPuzzleUtil Util;
    for (auto M : motifs()) {
        auto It = createWoodPieceWithMotif(M);
        Size S = It->getContentSize();
        Rect R(S / 2.f, CS - S);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Util.wallBiasedRandomPoint(R));
        Field->appendWoodPiece(It);
    }
}

void StoreForWorksheet::refinePlayCondition(GameBoard* Board, PlayField* Field) const {
    Super::refinePlayCondition(Board, Field);
}


END_NS_WOODENPUZZLES
