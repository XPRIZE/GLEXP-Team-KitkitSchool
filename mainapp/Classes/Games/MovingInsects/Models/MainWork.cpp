//
//  MainWork.cpp on Sep 5, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainWork.h"
#include <Common/Basic/CommentStream.h>


BEGIN_NS_MOVINGINSECTS

namespace {
}  // unnamed namespace


MainWork::MainWork()
: Operator()
, LeftExpr()
, RightExpr()
, Answer() {
}

MainWork::MainWork(const string& Operator,
                   int LeftExpr,
                   int RightExpr,
                   int Answer)
: Operator(Operator)
, LeftExpr(LeftExpr)
, RightExpr(RightExpr)
, Answer(Answer) {
}

MainWork MainWork::randomWork() {
    if (random(0, 1) == 0) {
        return randomPlusWork();
    }
    
    return randomMinusWork();
}

MainWork MainWork::randomPlusWork() {
    int X = random(1, 8);
    int Y = random(1, 8);
    int LHS = (9 - max(X, Y));
    int RHS = min(X, Y);

    return MainWork("+", LHS, RHS, LHS + RHS);
}

MainWork MainWork::randomMinusWork() {
    int X = random(1, 9);
    int Y = random(1, 9);
    int LHS = max(X, Y);
    int RHS = min(X, Y);

    return MainWork("-", LHS, RHS, LHS - RHS);
}

MainWork MainWork::fromInputStream(istream& IS) {
    string Type;

    if (IS >> Type)
    {
        if (Type == "PLUS") {
            int LHS, RHS;
            IS >> LHS >> RHS;
            return MainWork("+", LHS, RHS, LHS + RHS);
        }
        
        if (Type == "MINUS") {
            int LHS, RHS;
            IS >> LHS >> RHS;
            return MainWork("-", LHS, RHS, LHS - RHS);
        }
        
        if (Type == "RANDOM_PLUS")
            return MainWork::randomPlusWork();
        
        if (Type == "RANDOM_MINUS")
            return MainWork::randomMinusWork();
        
        if (Type == "RANDOM")
            return MainWork::randomWork();
    }
    
    // NB(xenosoz, 2016): Fallback to a random work
    CCLOGERROR("Bad work type(%s) in %s", Type.c_str(), __PRETTY_FUNCTION__);
    return MainWork::randomWork();
}

istream& operator>>(istream& IS, MainWork& Work) {
    Work = MainWork::fromInputStream(IS);
    return IS;
}

END_NS_MOVINGINSECTS