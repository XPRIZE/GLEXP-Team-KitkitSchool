//
//  SpellBall.h -- A ball with a spell on it
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "BallBase.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class SpellBall: public BallBase {
    typedef BallBase Super;

public:    
    function<void(Touch*, Event*)> OnDrag;
    
public:
    CREATE_FUNC(SpellBall);
    bool init() override;
    
protected:
    void clearInternals() override;
    void refreshChildNodes() override;
    void refreshChildComponents() override;
};


END_NS_SPELLING;