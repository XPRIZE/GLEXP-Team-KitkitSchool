//
//  Worksheet.h -- A collection of problem for LetterTrace
//  TodoSchool on Oct 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Worksheet.h"


BEGIN_NS_LETTERTRACE

size_t Worksheet::beginProblemID() const {
    return Problems.begin_index();
}

size_t Worksheet::endProblemID() const {
    return Problems.end_index();
}

Problem& Worksheet::problemByID(size_t ProblemID) {
    Problems.resize_for_index(ProblemID);
    return Problems.at(ProblemID);
}

Problem& Worksheet::randomProblem() {
    size_t ProblemID = random(Problems.begin_index() + 1, Problems.end_index()) - 1;
    return problemByID(ProblemID);
}

istream& operator>>(istream& IS, Worksheet& WS) {
    size_t ProblemID;
    if (IS >> ProblemID) {
        auto& P = WS.problemByID(ProblemID);
        IS >> P;
    }

    return IS;
}

END_NS_LETTERTRACE
