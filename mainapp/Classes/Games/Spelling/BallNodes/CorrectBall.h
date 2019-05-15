//
//  CorrectBall.h -- A chameleon ball for the correct answer
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "DummyBall.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING

class CorrectBall: public Node {
    typedef Node Super;
    BallSize TheBallSize;
    SoundEffect SoundForLetterName;

    DummyBall* YellowBall;
    DummyBall* BlueBall;
    
public:
    Chain<string> Text;

public:
    CREATE_FUNC(CorrectBall);
    CorrectBall();
    bool init(BallSize BS);
    
public:
    static Size defaultSize(BallSize BS);

private:
    void clearInternals();
    void refreshComponents();
    void refreshChildNodes();
    
    bool init() override { return false; }
    
};
END_NS_SPELLING
