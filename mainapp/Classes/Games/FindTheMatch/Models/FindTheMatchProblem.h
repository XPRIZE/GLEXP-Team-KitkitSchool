//
//  Problem.h on Sep 5, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/FindTheMatchNS.h"


BEGIN_NS_FINDTHEMATCH

class Problem {
public:
    size_t NumberOfChoices;
    size_t AnswerID;
    vector<string> Choices;
    
public:
    Problem();
    Problem(size_t NumberOfChoices,
            const string& AnswerStr,
            const string& Choice0,
            const string& Choice1,
            const string& Choice2,
            const string& Choice3);
    
    string assetForProblemCard() const;
    string assetForChoiceCard(size_t CardID) const;


public:
    static Problem randomWork();
    static Problem fromInputStream(istream& IS);
    
    friend istream& operator>>(istream& IS, Problem& Work);
};

END_NS_FINDTHEMATCH
