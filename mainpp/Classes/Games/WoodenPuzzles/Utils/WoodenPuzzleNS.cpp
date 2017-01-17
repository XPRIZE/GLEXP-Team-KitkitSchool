//
//  WoodenPuzzleNS.h -- Namespace management for WoodenPuzzles
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES

std::string itos(int I) { return TodoUtil::itos(I); }
int stoi(const std::string& S) { return TodoUtil::stoi(S); }

END_NS_WOODENPUZZLES