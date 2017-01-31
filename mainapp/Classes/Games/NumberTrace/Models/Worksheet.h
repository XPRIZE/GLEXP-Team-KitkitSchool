//
//  Worksheet.h -- A collection of problem for LetterTrace
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Problem.h"
#include "../Utils/MainNS.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_NUMBERTRACE

class Worksheet {
    OneBasedVector<Problem> Problems;
    
public:
    size_t size() const;
    int beginProblemID() const;
    int endProblemID() const;
    
    Problem& problemByID(size_t ProblemID);
    Problem& randomProblem();
    
public:
    friend istream& operator>>(istream& IS, Worksheet& WS);
};

END_NS_NUMBERTRACE