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
namespace lettermatching {

Problem::Problem() {
}
    
size_t Problem::size() const {
    return pieces.size();
}

istream& operator>>(istream& stream, Problem& problem) {
    Piece piece;
    stream >> piece;
    
    problem.pieces.push_back(piece);
    return stream;
}

}  // namespace lettermatching
}  // namespace todoschool