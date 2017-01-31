//
//  DummyBall.h -- A dummy ball for representing the correct answer / candidate
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "BallTraits.h"
#include "../Utils/MainNS.h"


BEGIN_NS_SPELLING

class DummyBall: public Node {
    typedef Node Super;
    
    BallSize TheBallSize;
    BallKind TheBallKind;

public:
    Chain<string> Text;

public:
    CREATE_FUNC(DummyBall);
    bool init(BallSize BS, BallKind BK);

private:
    void clearInternals();
    void refreshChildNodes();

    bool init() override { return false; }
};

END_NS_SPELLING
