//
//  MainNS.h -- Namespace management for FindTheMatch
//  TodoSchool on Oct 6, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "MainNS.h"


BEGIN_NS_FINDTHEMATCH

std::string itos(int I) { return TodoUtil::itos(I); }
int stoi(const std::string& S) { return TodoUtil::stoi(S); }

END_NS_FINDTHEMATCH