//
//  Shape.cpp
//  TodoSchool on Jun 5, 2018
//
//  Copyright (c) 2018 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Shape.hpp"
#include <string>

using namespace std;

Shape::Shape()
{
}

Shape::Shape(const std::vector<string>& attribute_keys_,
             const std::vector<string>& language_keys_,
             const std::vector<string>& group_keys_)
: attribute_keys(attribute_keys_)
, language_keys(language_keys_)
, group_keys(group_keys_)
{
}

std::istream& operator>>(std::istream& stream, Shape& shape)
{
    stream >> shape.key;
    
    for (const auto& k : shape.attribute_keys) {
        stream >> shape.attributes[k];
    }
    
    for (const auto& k : shape.language_keys) {
        stream >> shape.languages[k];
    }
    
    for (const auto& k : shape.group_keys) {
        stream >> shape.groups[k];
    }

    return stream;
}
