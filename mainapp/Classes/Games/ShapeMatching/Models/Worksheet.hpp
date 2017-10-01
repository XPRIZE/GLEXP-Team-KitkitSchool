//
//  Worksheet.hpp -- A collection of problems
//  TodoSchool on Oct 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Problem.hpp"
#include <istream>


namespace ShapeMatching {
    
    class Worksheet {
        std::vector<Problem> problems_;
        
    public:
        size_t size() const;
        int beginProblemID() const;
        int endProblemID() const;
        Problem& problemByID(size_t problemID);
        
    public:
        friend std::istream& operator>>(std::istream& stream, Worksheet& sheet);
    };
    
}  // namespace ShapeMatching
