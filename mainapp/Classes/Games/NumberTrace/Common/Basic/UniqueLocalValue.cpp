//
//  UniqueLocalValue.cpp -- Unique value generator
//  TodoSchool on Aug 17, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "UniqueLocalValue.h"
#include <sstream>


namespace todoschool {
    
UniqueLocalValue::UniqueLocalValue()
    : V(new char())
{
}
    
void UniqueLocalValue::reset() {
    V.reset(new char());
}
    
bool UniqueLocalValue::operator==(const UniqueLocalValue& ULV) {
    return V.get() == ULV.V.get();
}

bool UniqueLocalValue::operator!=(const UniqueLocalValue& ULV) {
    return V.get() != ULV.V.get();
}

void* UniqueLocalValue::asPointer() {
    return V.get();
}

std::string UniqueLocalValue::asString() {
    std::stringstream SS;
    SS << V.get();
    return SS.str();
}

}  // namespace todoschool