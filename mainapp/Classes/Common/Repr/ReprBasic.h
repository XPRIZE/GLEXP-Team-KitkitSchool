//
//  ReprBasic.h on May 09, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_REPR_REPRBASIC_H
#define TODOSCHOOL_REPR_REPRBASIC_H

#include <string>

#define REPR(OBJ) CCLOG(#OBJ ": %s", repr((OBJ)).c_str())
#define REPR_HERE CCLOG("__PRETTY_FUNCTION__: %s", __PRETTY_FUNCTION__)

namespace todoschool {

template<class ReprClass>
std::string repr(const ReprClass& object) {
    return object.__repr__();
}
   
template<> std::string repr<bool>(const bool& Value);
template<> std::string repr<float>(const float& Value);
template<> std::string repr<std::string>(const std::string& S);

}  // namespace todoschool
#endif  // !defined(TODOSCHOOL_REPR_REPRBASIC_H)
