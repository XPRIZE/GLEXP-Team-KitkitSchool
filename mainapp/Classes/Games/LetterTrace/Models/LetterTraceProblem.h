//
//  Problem.h -- A problem for LetterTrace
//  TodoSchool on Oct 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/LetterTraceNS.h"


BEGIN_NS_LETTERTRACE

class Problem {
public:
    string TraceText;
    bool BonusVideoEnabled;

    string FullText;
    string VideoFileName;
    
public:
    Problem();
    Problem(const string& TraceText,
            bool BonusVideoEnabled,
            const string& FullText,
            const string& VideoFileName);

public:
    static Problem fromInputStream(istream& IS);
    friend istream& operator>>(istream& IS, Problem& P);
};

END_NS_LETTERTRACE
