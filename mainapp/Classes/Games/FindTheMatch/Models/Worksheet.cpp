//
//  Worksheet.cpp -- Main worksheet
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "Worksheet.h"


BEGIN_NS_FINDTHEMATCH

size_t Worksheet::size() const {
    return Problems.size();
}

int Worksheet::beginProblemID() const {
    return (int)Problems.begin_index();
}

int Worksheet::endProblemID() const {
    return (int)Problems.end_index();
}

Problem& Worksheet::problemByID(size_t WorkID) {
    return Problems.at(WorkID);
}

istream& operator>>(istream& IS, Worksheet& Sheet) {
    size_t ProblemID;
    
    if (IS >> ProblemID) {
        Sheet.Problems.resize_for_index(ProblemID);
        auto& P = Sheet.problemByID(ProblemID);
        IS >> P;
    }

    return IS;
}

END_NS_FINDTHEMATCH
