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
namespace wordmachine {

Problem Problem::fromInputStream(istream &stream) {
    Problem problem;
    string typeString;

    stream >> typeString >> problem.word >> problem.goodImage >> problem.badImage >> problem.sound;
    if (!stream) {
        CCLOGERROR("Unexpected end of stream in %s", __PRETTY_FUNCTION__);
        return Problem();
    }
    
    if (typeString == "LEGIT") {
        problem.type = TypeKind::Legit;
    }
    else if (typeString == "MADEUP") {
        problem.type = TypeKind::Madeup;
    }
    else {
        CCLOGERROR("Unexpected type string %s in %s", typeString.c_str(), __PRETTY_FUNCTION__);
        return Problem();
    }
    
    return problem;
}
    
bool Problem::legitType() const {
    return type == TypeKind::Legit;
}

bool Problem::madeupType() const {
    return type == TypeKind::Legit;
}

istream& operator>>(istream& stream, Problem& problem) {
    problem = Problem::fromInputStream(stream);
    return stream;
}

}  // namespace wordmachine
}  // namespace todoschool