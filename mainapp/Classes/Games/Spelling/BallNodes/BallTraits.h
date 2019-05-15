//
//  BallTraits.h -- Ball types
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING

// NB(xenosoz, 2016): BallSize does *not* match 1:1 to the design resource name.
//   You may want to look at DummyBall.cpp for details.

enum class BallSize {
    // NB(xenosoz, 2016): Used for word length [5, inf)
    Small,
    
    // NB(xenosoz, 2016): Used for word length [3, 5)
    Large,
};

enum class BallKind {
    CorrectAnswer,
    ActiveAnswer,
    PassiveAnswer,
};

END_NS_SPELLING
