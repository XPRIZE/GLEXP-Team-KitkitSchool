//
//  Work.h on Aug 24, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class Work {
public:
    string WorkID;
    string Word;
    
public:
    Work();
    Work(const string& WorkID, const string& Word);
};

END_NS_SPELLING;