//
//  Problem.cpp -- A problem for the game
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Problem.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


namespace todoschool {
namespace counting {

Problem::Problem()
: totalCount(0)
, stoneCount(0)
, tallyCount(0)
{
}

Problem::Problem(int stoneCount, int tallyCount)
: stoneCount(stoneCount)
, tallyCount(tallyCount)
{
    totalCount = stoneCount + tallyCount;
}

Problem Problem::fromInputStream(istream &stream) {
    int stoneCount;
    int tallyCount;

    if (!(stream >> stoneCount >> tallyCount)) {
        CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
        return Problem();
    }
    
    return Problem(stoneCount, tallyCount);
}

istream& operator>>(istream& stream, Problem& problem) {
    problem = Problem::fromInputStream(stream);
    return stream;
}

}  // namespace counting
}  // namespace todoschool
