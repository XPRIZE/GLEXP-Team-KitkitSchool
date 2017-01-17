//
//  MainWork.h on Sep 5, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_MOVINGINSECTS

class MainWork {
public:
    string Operator;
    int LeftExpr;
    int RightExpr;    
    int Answer;
    
public:
    MainWork();
    MainWork(const string& Operator,
             int LeftExpr,
             int RightExpr,
             int Answer);
    
public:
    static MainWork randomWork();
    static MainWork randomPlusWork();
    static MainWork randomMinusWork();
    static MainWork fromInputStream(istream& IS);
    
    friend istream& operator>>(istream& IS, MainWork& Work);
};

END_NS_MOVINGINSECTS