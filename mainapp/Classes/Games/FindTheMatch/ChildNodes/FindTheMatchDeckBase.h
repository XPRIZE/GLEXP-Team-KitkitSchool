//
//  DeckBase.h on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "FindTheMatchCard.h"
#include "../Utils/FindTheMatchNS.h"

BEGIN_NS_FINDTHEMATCH


class DeckBase: public Node {
    typedef Node Super;
    
public:
    CREATE_FUNC(DeckBase);
    DeckBase();
    bool init() override;

    // NB(xenosoz, 2016): Move the card to this deckbase instantly.
    //   The card should be "connceted" to world space before the function call.
    void catchCard(Card* C);

private:
    void clearInternals();
};


END_NS_FINDTHEMATCH
