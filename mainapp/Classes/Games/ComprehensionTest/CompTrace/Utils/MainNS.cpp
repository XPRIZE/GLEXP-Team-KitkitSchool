//
//  MainNS.cpp -- Namespace management for CompTrace
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainNS.h"


BEGIN_NS_COMPTRACE

std::string itos(int I) { return TodoUtil::itos(I); }
int stoi(const std::string& S) { return TodoUtil::stoi(S); }

END_NS_COMPTRACE
