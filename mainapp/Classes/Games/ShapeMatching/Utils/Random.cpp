//
//  Random.cpp -- Random number generation
//  TodoSchool on Aug 8, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Random.hpp"



Random& Random::shared() {
    static Random sharedInstance;
    return sharedInstance;
}

Random::Random()
: gen(rd())
{
}

