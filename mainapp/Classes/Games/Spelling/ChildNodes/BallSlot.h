//
//  BallSlot.h -- Empty/full ball slot for GameBoard
//  TodoSchool on Sep 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../BallNodes/CorrectBall.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING

class BallSlot: public Node {
    typedef Node Super;
    BallSize TheBallSize;
    
    CorrectBall* TheBall;

public:
    Chain<string> BallText;
    function<void()> OnSlotDidFill;

public:
    CREATE_FUNC(BallSlot);
    BallSlot();
    bool init(BallSize BS);
    
    bool empty();
    void ejectBall();
    void fillSlot();

public:
    static Size defaultSize(BallSize BS);

private:
    void clearInternals();
    void refreshChildNodes();

    bool init() override { assert(false); return false; }
};

END_NS_SPELLING
