//
//  AnswerBall.h -- Blue answer ball with flexible size
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "DummyBall.h"
#include "../Utils/MainNS.h"


BEGIN_NS_SPELLING

class AnswerBall: public Node {
    typedef Node Super;
    BallSize TheBallSize;
    SoundEffect SoundForLetterName;
    SoundEffect SoundForHit;
    
    DummyBall* ActiveBall;
    DummyBall* PassiveBall;
    
public:
    Chain<size_t> BallID;
    Chain<string> Text;
    Chain<GLubyte> Opacity;
    Chain<Point> PositionInWorldSpace;
    Chain<Point> HomePositionInWorldSpace;
    Chain<Point> PlatformPositionInLocalSpace;

    function<void(Touch*, Event*)> OnDragDidBegin;
    function<void(Touch*, Event*)> OnDrag;
    function<void(Touch*, Event*)> OnDragDidEnd;

public:
    CREATE_FUNC(AnswerBall);
    AnswerBall();
    bool init(BallSize BS);
    
    using Super::setPosition;
    void setPosition(float x, float y) override;
    void playHitSound();
    
public:
    static Size defaultSizeForActiveBall(BallSize BS);
    static Size defaultSizeForPassiveBall(BallSize BS);

private:
    void clearInternals();
    void refreshComponents();
    void refreshChildNodes();
    
    void makeActive();
    void makePassive();
    
    bool init() override { assert(false); return false; }
};

END_NS_SPELLING