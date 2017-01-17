//
//  Problem.h -- A problem for WordTrace
//  TodoSchool on Jul 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Problem.h"


BEGIN_NS_WORDTRACE


Problem::Problem()
: ProblemID(0)
{
}

Problem::Problem(std::string Text,
                 std::string VideoFileName,
                 std::string TeaserFileName)
: ProblemID(0)
, Text(Text)
, VideoFileName(VideoFileName)
, TeaserFileName(TeaserFileName)
{
}

Problem Problem::fromInputStream(istream &IS) {
    Problem P;
    
    if (!(IS >> P.Text >> P.VideoFileName >> P.TeaserFileName)) {
        CCLOGERROR("Unexpected input stream error in %s", __PRETTY_FUNCTION__);
        return Problem();
    }
    
    return P;
}

istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}

END_NS_WORDTRACE
