//
//  Worksheet.cpp -- A collection of Problems
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WordTraceWorksheet.h"


BEGIN_NS_WORDTRACE

size_t Worksheet::size() const {
    return Problems.size();
}

int Worksheet::beginProblemID() const {
    return (int)Problems.begin_index();
}

int Worksheet::endProblemID() const {
    return (int)Problems.end_index();
}

Problem& Worksheet::problemByID(int ProblemID) {
    return Problems.at(ProblemID);
}

Problem& Worksheet::randomProblem() {
    auto ProblemID = (int)random(Problems.begin_index(), Problems.end_index() - 1);
    return problemByID(ProblemID);
}

istream& operator>>(istream& IS, Worksheet& WS) {
    int ProblemID;
    if (IS >> ProblemID) {
        WS.Problems.resize_for_index(ProblemID);
        auto& P = WS.problemByID(ProblemID);
        IS >> P;
        P.ProblemID = ProblemID;
    }
    
    return IS;
}

END_NS_WORDTRACE

