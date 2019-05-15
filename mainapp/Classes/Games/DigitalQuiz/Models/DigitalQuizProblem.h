//
//  Problem.h -- A problem for the game
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/DigitalQuizNS.h"
#include <Common/Basic/OneBasedVector.h>


BEGIN_NS_DIGITALQUIZ

class Problem {
public:
    string Indicator;
    string GameType;
    string Instruction;

    string Question;
    float OpeningDuration;
    string OpeningSound;
    string ButtonSound;
    string Text;
    
    string Answer;
    OneBasedVector<string> Choices;

public:
    static Problem fromInputStream(istream& IS);
    friend istream& operator>>(istream& IS, Problem& P);

};

END_NS_DIGITALQUIZ
