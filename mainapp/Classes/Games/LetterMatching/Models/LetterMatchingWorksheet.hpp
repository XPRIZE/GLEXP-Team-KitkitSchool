//
//  Worksheet.hpp -- A collection of problems
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "LetterMatchingProblem.hpp"
#include <Common/Basic/OneBasedVector.h>
#include <istream>


namespace todoschool {
namespace lettermatching {

class Worksheet {
    OneBasedVector<Problem> problems_;
    
public:
    size_t size() const;
    int beginProblemID() const;
    int endProblemID() const;
    Problem& problemByID(size_t problemID);
    
public:
    friend std::istream& operator>>(std::istream& stream, Worksheet& sheet);
};

}  // namespace lettermatching
}  // namespace todoschool
