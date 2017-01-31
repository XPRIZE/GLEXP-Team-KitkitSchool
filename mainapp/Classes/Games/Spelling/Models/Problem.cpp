//
//  Problem.cpp -- A problem for the game
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Problem.h"


BEGIN_NS_SPELLING

bool Problem::letterBecomesChoice(size_t Index) const {
    // NB(xenosoz, 2016): Regard all out-of-bound masks as '*'.
    return Index >= WordMask.size() || WordMask[Index] == '*';
}

Problem Problem::fromInputStream(istream &IS) {
    Problem P;
    
    IS >> P.Word;
    IS >> P.WordMask >> P.ConsonantChoiceCount >> P.VowelChoiceCount;
    IS >> P.ImageFilename >> P.SoundDuration;

    if (!IS) {
        CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
        return Problem();
    }
    
    return P;
}

istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}

END_NS_SPELLING
