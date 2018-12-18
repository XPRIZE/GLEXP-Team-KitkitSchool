//
//  Problem.hpp -- A problem for the game
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "LetterMatchingPiece.hpp"
#include <istream>
#include <vector>


namespace todoschool {
namespace lettermatching {

class Problem {
public:
    std::vector<Piece> pieces;

public:
    Problem();
    size_t size() const;

public:
    friend std::istream& operator>>(std::istream& stream, Problem& problem);
};

}  // namespace lettermatching
}  // namespace todoschool
