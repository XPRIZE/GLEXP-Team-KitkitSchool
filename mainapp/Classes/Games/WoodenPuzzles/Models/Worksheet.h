//
//  Worksheet.h -- A collection of problems
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Problem.h"
#include "../Utils/WoodenPuzzleNS.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_WOODENPUZZLES

class Worksheet {
public:
    enum class PuzzleType {
        Free,
        Ordered,
        RandomOrdered,
    };

    OneBasedVector<Problem> Problems;
    PuzzleType ThePuzzleType;
    string GameID;
    string BackgroundFilename;
    
    map<string, size_t> ProblemIndexByMotif;

public:
    size_t size() const;
    int beginProblemID() const;
    int endProblemID() const;
    const Problem& problemByID(size_t ProblemID) const;
    const Problem& problemByMotif(const string& M) const;

    Problem& problemByID(size_t ProblemID);
    Problem& problemByMotif(const string& M);
    
    void shuffleProblems();
    
public:
    friend istream& operator>>(istream& IS, Worksheet& WS);
};

END_NS_WOODENPUZZLES
