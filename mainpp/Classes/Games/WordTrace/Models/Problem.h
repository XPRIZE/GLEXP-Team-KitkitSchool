//
//  Problem.h -- A problem for WordTrace
//  TodoSchool on Jul 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_WORDTRACE

class Problem {
public:
    // NB(xenosoz, 2016): ProblemID is given from the external world
    //   *after* the instance is created.
    int ProblemID;

    std::string Text;
    std::string VideoFileName;
    std::string TeaserFileName;
    
public:
    Problem();
    Problem(std::string Text,
            std::string VideoFileName,
            std::string TeaserFileName);
    
public:
    static Problem fromInputStream(istream& IS);
    friend istream& operator>>(istream& IS, Problem& P);
};

END_NS_WORDTRACE
