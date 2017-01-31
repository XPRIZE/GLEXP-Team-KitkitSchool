//
//  CommentStream.h -- (Input) stream which ignores '#' comment strings
//  TodoSchool on Sep 30, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <string>
#include <sstream>
#include <locale>
#include <Common/Basic/TodoSchoolNS.h>

BEGIN_NS_TODOSCHOOL


class CommentStream {
    std::istringstream ISS;

public:
    CommentStream();
    CommentStream(const std::string& S);
    
    operator bool() const;
    std::locale getloc() const;
    std::locale imbue(const std::locale& loc);
    
public:
    template<typename T>
    CommentStream& operator>>(T& Target) {
        ISS >> Target;
        return *this;
    }
};

END_NS_TODOSCHOOL