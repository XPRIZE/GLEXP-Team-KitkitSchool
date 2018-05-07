//
//  ReprBasic.cpp on Jul 12, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ReprBasic.h"
#include <sstream>


namespace todoschool {

template<> std::string repr<bool>(const bool& Value) {
    std::stringstream SS;
    SS << "bool(";
    SS << (Value ? "true" : "false");
    SS << ")";
    return SS.str();
}

template<> std::string repr<float>(const float& Value) {
    std::stringstream SS;
    SS << "float(";
    SS << Value;
    SS << ")";
    return SS.str();
}

template<> std::string repr<std::string>(const std::string& S) {
    std::stringstream SS;
    SS << "std::string(";
    SS << S;
    SS << ")";
    return SS.str();
}

}  // namespace todoschool
