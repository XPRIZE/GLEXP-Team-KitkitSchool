//
//  DummyBall.h -- A ball with a spell on it
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "BallBase.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class DummyBall: public BallBase {
    typedef BallBase Super;

public:
    CREATE_FUNC(DummyBall);
    bool init() override;
    
protected:
    void clearInternals() override;
    void refreshChildNodes() override;
    void refreshChildComponents() override;
};


END_NS_SPELLING;