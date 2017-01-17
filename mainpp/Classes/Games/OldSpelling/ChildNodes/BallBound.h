//
//  BallBound.h -- Boundary for SpellBall
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class BallBound: public Node {
    typedef Node Super;

public:
    CREATE_FUNC(BallBound);
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
    void refreshChildComponents();
};

END_NS_SPELLING;