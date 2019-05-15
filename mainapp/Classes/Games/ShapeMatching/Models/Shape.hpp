//
//  Shape.hpp
//  TodoSchool on Jun 5, 2018
//
//  Copyright (c) 2018 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include <string>
#include <vector>
#include <map>


class Shape
{
public:
    std::vector<std::string> attribute_keys;
    std::vector<std::string> language_keys;
    std::vector<std::string> group_keys;
    
    std::string key;
    std::map<std::string, std::string> attributes;
    std::map<std::string, std::string> languages;
    std::map<std::string, std::string> groups;


public:
    Shape();
    Shape(const std::vector<std::string>& attribute_keys,
          const std::vector<std::string>& language_keys,
          const std::vector<std::string>& group_keys);
    
    friend std::istream& operator>>(std::istream& stream, Shape& shape);

};
