//
//  Worksheet.cpp -- A collection of problems
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzlesWorksheet.h"


BEGIN_NS_WOODENPUZZLES

size_t Worksheet::size() const {
    return Problems.size();
}

int Worksheet::beginProblemID() const {
    return (int)Problems.begin_index();
}

int Worksheet::endProblemID() const {
    return (int)Problems.end_index();
}

const Problem& Worksheet::problemByID(size_t ProblemID) const {
    return Problems.at(ProblemID);
}

const Problem& Worksheet::problemByMotif(const string& M) const {
    return Problems.at(ProblemIndexByMotif.at(M));
}


Problem& Worksheet::problemByID(size_t ProblemID) {
    return Problems.at(ProblemID);
}

Problem& Worksheet::problemByMotif(const string& M) {
    return Problems.at(ProblemIndexByMotif.at(M));
}

void Worksheet::shuffleProblems() {
    // NB(xenosoz, 2016): The Problem class itself doesn't aware of its ProblemID now.
    //   But we may need to update problem IDs in the future.
    shuffle(Problems);
    
    // NB(xenosoz, 2016): Update index by Motif.
    for (auto I = Problems.begin_index(), E = Problems.end_index(); I < E; ++I) {
        auto& ProblemID = I;
        auto& P = Problems[I];
        ProblemIndexByMotif[P.Motif] = ProblemID;
    }
}

istream& operator>>(istream& IS, Worksheet& WS) {
    [&] {
        string Key;
        if (!(IS >> Key))
            return;

        if (Key == "__GAME_ID__") {
            string PuzzleType, GameID, Background;
            string _3, _4, _5, _6;
            
            if (IS >> PuzzleType >> GameID >> Background >> _3 >> _4 >> _5 >> _6) {
                WS.ThePuzzleType = [&] {
                    if (PuzzleType == "FreePuzzle") {
                        return Worksheet::PuzzleType::Free;
                    }
                    if (PuzzleType == "OrderedPuzzle") {
                        return Worksheet::PuzzleType::Ordered;
                    }
                    if (PuzzleType == "RandomOrderedPuzzle") {
                        return Worksheet::PuzzleType::RandomOrdered;
                    }

                    CCLOGERROR("Unsupported puzzle type (%s) in %s",
                               PuzzleType.c_str(),
                               __PRETTY_FUNCTION__);

                    return Worksheet::PuzzleType::Free;
                }();
                WS.GameID = GameID;
                WS.BackgroundFilename = Background;
            }
            else {
                CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
            }

            return;
        }

        size_t ProblemID = (size_t)woodenpuzzles::stoi(Key);
        WS.Problems.resize_for_index(ProblemID);
        auto& P = WS.problemByID(ProblemID);
        IS >> P;
        
        // NB(xenosoz, 2016): Make index by Motif.
        WS.ProblemIndexByMotif[P.Motif] = ProblemID;
    }();
    
    return IS;
}

END_NS_WOODENPUZZLES
