//
//  Symbol.h on Jul 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <string>
#include <vector>
#include <set>


namespace todoschool {
namespace starfall {

using SymbolType = std::string;
using SymbolRowType = std::vector<SymbolType>;
using SymbolLayoutType = std::vector<SymbolRowType>;

using SymbolSetType = std::set<SymbolType>;

}  // namespace starfall
}  // namespace todoschool
