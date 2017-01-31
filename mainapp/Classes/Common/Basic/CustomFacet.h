//
//  CustomFacet.h -- Customize separator for streams.
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include <locale>
#include <string>


namespace todoschool {

class CustomFacet: public std::ctype<char> {
public:
    typedef std::ctype<char> Base;
    typedef Base::char_type char_type;
    
private:
    Base::mask table_[256];
    
public:
    CustomFacet(const std::locale& loc);

    void setSpaceFlagsFor(const std::string& chars);
    void clearSpaceFlagsFor(const std::string& chars);
};

}  // namespace todoschool