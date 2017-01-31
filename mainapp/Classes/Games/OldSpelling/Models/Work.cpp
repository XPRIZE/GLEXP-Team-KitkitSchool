//
//  Work.cpp on Aug 24, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "Work.h"


BEGIN_NS_SPELLING;

Work::Work() {}

Work::Work(const string& WorkID, const string& Word)
    : WorkID(WorkID)
    , Word(Word)
{
}

END_NS_SPELLING;
