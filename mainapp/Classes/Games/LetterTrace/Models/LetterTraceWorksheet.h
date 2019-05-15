//
//  Worksheet.h -- A collection of problem for LetterTrace
//  TodoSchool on Oct 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "LetterTraceProblem.h"
#include "../Utils/LetterTraceNS.h"


BEGIN_NS_LETTERTRACE

class Worksheet {
    OneBasedVector<Problem> Problems;
    
public:
    // NB(xenosoz, 2016): Half open interval [begin, end).
    size_t beginProblemID() const;
    size_t endProblemID() const;

    Problem& problemByID(size_t ProblemID);
    Problem& randomProblem();
    
public:
    friend istream& operator>>(istream& IS, Worksheet& WS);
};

END_NS_LETTERTRACE
