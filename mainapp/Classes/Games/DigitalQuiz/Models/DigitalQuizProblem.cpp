//
//  Problem.cpp -- A problem for the game
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "DigitalQuizProblem.h"


BEGIN_NS_DIGITALQUIZ

Problem Problem::fromInputStream(istream &IS) {
    Problem P;
    string C1, C2, C3, C4, C5;

    IS >> P.Indicator >> P.GameType >> P.Instruction;
    IS >> P.Question;
    IS >> P.OpeningDuration >> P.OpeningSound >> P.ButtonSound;
    IS >> P.Text >> P.Answer >> C1 >> C2 >> C3 >> C4 >> C5;
    
    if (!IS) {
        CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
        return Problem();
    }
    
    // NB(xenosoz, 2016): Dirty codes right ahead!
    size_t EndSaneIndex = 0;
    if (C1 != "-") EndSaneIndex = 1; else C1.clear();
    if (C2 != "-") EndSaneIndex = 2; else C2.clear();
    if (C3 != "-") EndSaneIndex = 3; else C3.clear();
    if (C4 != "-") EndSaneIndex = 4; else C4.clear();
    if (C5 != "-") EndSaneIndex = 5; else C5.clear();

    if (EndSaneIndex >= 1) P.Choices.push_back(C1);
    if (EndSaneIndex >= 2) P.Choices.push_back(C2);
    if (EndSaneIndex >= 3) P.Choices.push_back(C3);
    if (EndSaneIndex >= 4) P.Choices.push_back(C4);
    if (EndSaneIndex >= 5) P.Choices.push_back(C5);
    
    return P;
}

istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}

END_NS_DIGITALQUIZ
