//
//  NumberTraceNS.cpp -- Namespace management for NumberTrace
//  TodoSchool on Oct 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "NumberTraceNS.h"


BEGIN_NS_NUMBERTRACE

std::string itos(int I) { return TodoUtil::itos(I); }
int stoi(const std::string& S) { return TodoUtil::stoi(S); }

END_NS_NUMBERTRACE
