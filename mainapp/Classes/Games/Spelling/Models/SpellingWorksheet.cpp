//
//  Worksheet.cpp -- A collection of Problems
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellingWorksheet.h"


BEGIN_NS_SPELLING

size_t Worksheet::size() const {
    return Problems.size();
}

int Worksheet::beginProblemID() const {
    return Problems.begin_index();
}

int Worksheet::endProblemID() const {
    return Problems.end_index();    
}

Problem& Worksheet::problemByID(size_t ProblemID) {
    return Problems.at(ProblemID);
}

istream& operator>>(istream& IS, Worksheet& WS) {
    size_t ProblemID;
    if (IS >> ProblemID) {
        WS.Problems.resize_for_index(ProblemID);
        auto& P = WS.problemByID(ProblemID);
        IS >> P;
    }

    return IS;
}

END_NS_SPELLING

