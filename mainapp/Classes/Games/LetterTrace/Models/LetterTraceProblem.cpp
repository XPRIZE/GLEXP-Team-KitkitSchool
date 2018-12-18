//
//  Problem.cpp -- A problem for LetterTrace
//  TodoSchool on Oct 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LetterTraceProblem.h"


BEGIN_NS_LETTERTRACE

Problem::Problem() {
}

Problem::Problem(const string& TraceText,
                 bool BonusVideoEnabled,
                 const string& FullText,
                 const string& VideoFileName)
: TraceText(TraceText)
, BonusVideoEnabled(BonusVideoEnabled)
, FullText(FullText)
, VideoFileName(VideoFileName)
{
}

Problem Problem::fromInputStream(istream& IS) {
    string TraceText, FullText, Video;
    if (IS >> TraceText >> FullText >> Video) {
        bool BonusVideoEnabled = (FullText != "-" && Video != "-");
        return Problem(TraceText, BonusVideoEnabled, FullText, Video);
    }

    return Problem();
}

istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}

END_NS_LETTERTRACE
