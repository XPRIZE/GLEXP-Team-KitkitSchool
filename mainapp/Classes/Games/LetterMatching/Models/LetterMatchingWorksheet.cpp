//
//  Worksheet.cpp -- A collection of Problems
//  TodoSchool on Oct 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "LetterMatchingWorksheet.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


namespace todoschool {
namespace lettermatching {

size_t Worksheet::size() const {
    return problems_.size();
}

int Worksheet::beginProblemID() const {
    return (int)problems_.begin_index();
}

int Worksheet::endProblemID() const {
    return (int)problems_.end_index();
}

Problem& Worksheet::problemByID(size_t problemID) {
    return problems_.at(problemID);
}

istream& operator>>(istream& stream, Worksheet& sheet) {
    size_t problemID;
    
    if (stream >> problemID) {
        sheet.problems_.resize_for_index(problemID);
        auto& problem = sheet.problemByID(problemID);
        stream >> problem;
    }
    
    return stream;
}

}  // namespace lettermatching
}  // namespace todoschool
