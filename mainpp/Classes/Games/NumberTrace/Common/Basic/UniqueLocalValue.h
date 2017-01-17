//
//  UniqueLocalValue.h -- Unique value generator
//  TodoSchool on Aug 17, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016):
//   Create a value unique in this process (not over the network).
//   The value will be unique as long as the object is alive.

#pragma once

#include <string>
#include <memory>


namespace todoschool {

class UniqueLocalValue {
    std::shared_ptr<char> V;

public:
    UniqueLocalValue();
    void reset();
    
    bool operator==(const UniqueLocalValue& ULV);
    bool operator!=(const UniqueLocalValue& ULV);

    void* asPointer();
    std::string asString();    
};

}  // namespace todoschool
