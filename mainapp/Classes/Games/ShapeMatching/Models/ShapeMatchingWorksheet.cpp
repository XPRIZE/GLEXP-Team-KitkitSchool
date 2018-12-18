//
//  Worksheet.cpp -- A collection of Problems
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ShapeMatchingWorksheet.hpp"
#include <cocos/cocos2d.h>

using namespace std;
using namespace cocos2d;


namespace ShapeMatching {
    
    size_t Worksheet::size() const {
        return problems_.size();
    }
    
    int Worksheet::beginProblemID() const {
        return (int)1;
    }
    
    int Worksheet::endProblemID() const {
        return (int)problems_.size();
    }
    
    Problem& Worksheet::problemByID(size_t problemID) {
        return problems_.at(problemID-1);
    }
    
    istream& operator>>(istream& stream, Worksheet& sheet) {
        size_t problemID;
        
        if (stream >> problemID) {
            if (problemID > sheet.problems_.size()) {
                sheet.problems_.resize(problemID);
            }
            auto& problem = sheet.problemByID(problemID);
            stream >> problem;
        }
        
        return stream;
    }
    
}  // namespace ShapeMatching
