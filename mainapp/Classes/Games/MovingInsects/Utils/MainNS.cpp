//
//  MainNS.cpp on Sep 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainNS.h"


BEGIN_NS_MOVINGINSECTS

std::string itos(int I) { return TodoUtil::itos(I); }
int stoi(const std::string& S) { return TodoUtil::stoi(S); }

END_NS_MOVINGINSECTS