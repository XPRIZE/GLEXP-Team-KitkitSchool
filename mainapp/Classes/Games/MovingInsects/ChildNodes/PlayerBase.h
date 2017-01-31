//
//  PlayerBase.h on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Card.h"
#include "../Utils/MainNS.h"


BEGIN_NS_MOVINGINSECTS

class PlayerBase: public Node {
    typedef Node Super;

public:
    Chain<size_t> SlotCount;


public:
    CREATE_FUNC(PlayerBase);
    PlayerBase();
    bool init() override;
    
    // NB(xenosoz, 2016): Move the card to this playerbase with some animation.
    //   The card should be "connceted" to world space before the function call.
    FiniteTimeAction* actionForPullCardWithSlotIndex(Card* C, size_t Index);
    FiniteTimeAction* actionForShakeCardWithSlotIndex(Card* C, size_t Index);

    void pullCardWithSlotIndex(Card* C, size_t Index);

private:
    void clearInternals();
    
    Optional<Point> pointInCardParent(Card* C, size_t Index);
};

END_NS_MOVINGINSECTS