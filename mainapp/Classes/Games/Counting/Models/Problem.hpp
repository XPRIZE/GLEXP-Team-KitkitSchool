//
//  Problem.hpp -- A problem for the game
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <istream>


namespace todoschool {
namespace counting {

class Problem {
public:
    int totalCount;
    int stoneCount;
    int tallyCount;

public:
    Problem();
    Problem(int stoneCount, int tallyCount);

public:
    static Problem fromInputStream(std::istream& stream);
    friend std::istream& operator>>(std::istream& stream, Problem& problem);
};

}  // namespace counting
}  // namespace todoschool
