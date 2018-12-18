//
//  GameBoard.h on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "FindTheMatchCard.h"
#include "../Utils/FindTheMatchNS.h"

BEGIN_NS_FINDTHEMATCH


class GameBoard: public Node {
    typedef Node Super;

public:
    CREATE_FUNC(GameBoard);
    GameBoard();
    bool init() override;

    // NB(xenosoz, 2016): Move the card to this gameboard instantly.
    //   The card should be "connceted" to world space before the function call.
    void catchCardToProblemSlot(Card* C);

    // NB(xenosoz, 2016): Move the card to this gameboard with some animation.
    //   The card should be "connceted" to world space before the function call.
    FiniteTimeAction* actionForPullCardToAnswerSlot(Card* C);
    void pullCardToAnswerSlot(Card* C);

private:
    void clearInternals();
    void refreshChildNodes();
};


END_NS_FINDTHEMATCH
