//
//  Problem.h -- A problem for the game
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/TutorialTraceNS.h"


BEGIN_NS_TUTORIALTRACE

class Problem {
public:
    string Word;
    string WordMask;
    int ConsonantChoiceCount;
    int VowelChoiceCount;
    string ImageFilename;
    float SoundDuration;

public:
    bool letterBecomesChoice(size_t Index) const;

public:
    static Problem fromInputStream(istream& IS);
    friend istream& operator>>(istream& IS, Problem& P);

};

END_NS_TUTORIALTRACE
