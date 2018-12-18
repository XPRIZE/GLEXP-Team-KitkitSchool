//
//  Problem.hpp -- A problem for the game
//  TodoSchool on Oct 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <istream>


namespace todoschool {
namespace wordmachine {

class Problem {
public:
    enum class TypeKind {
        Legit,
        Madeup,
    };

public:
    TypeKind type;
    std::string word;
    std::string goodImage;
    std::string badImage;
    std::string sound;
    
public:
    bool legitType() const;
    bool madeupType() const;

public:
    static Problem fromInputStream(std::istream& stream);
    friend std::istream& operator>>(std::istream& stream, Problem& problem);
};

}  // namespace wordmachine
}  // namespace todoschool
