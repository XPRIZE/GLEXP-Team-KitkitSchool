//
//  TraceFieldNS.cpp -- Namespace management for TraceField
//  TodoSchool on Sep 8, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

std::string itos(int I) { return TodoUtil::itos(I); }
int stoi(const std::string& S) { return TodoUtil::stoi(S); }

END_NS_TRACEFIELD
