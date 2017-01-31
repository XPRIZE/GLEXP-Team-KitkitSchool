//
//  Worksheet.h -- Main worksheet
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"
#include "Problem.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_FINDTHEMATCH

class Worksheet {
    OneBasedVector<Problem> Problems;
    
public:
    size_t size() const;
    int beginProblemID() const;
    int endProblemID() const;
    Problem& problemByID(size_t ProblemID);
    
public:
    friend istream& operator>>(istream& IS, Worksheet& Sheet);
};

END_NS_FINDTHEMATCH