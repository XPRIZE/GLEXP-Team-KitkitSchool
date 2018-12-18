//
//  Problem.cpp -- A problem for the game
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ShapeMatchingProblem.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


namespace ShapeMatching {
    
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
    
}  // namespace ShapeMatching
