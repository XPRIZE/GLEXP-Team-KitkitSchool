//
//  Problem.hpp -- A problem for the game
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "NumberMatchingPiece.hpp"
#include <vector>


namespace todoschool {
namespace numbermatching {

class Problem {
public:
    std::vector<Piece> pieces;

public:
    Problem();
    size_t size() const;

public:
    friend std::istream& operator>>(std::istream& stream, Problem& problem);
};

}  // namespace numbermatching
}  // namespace todoschool
