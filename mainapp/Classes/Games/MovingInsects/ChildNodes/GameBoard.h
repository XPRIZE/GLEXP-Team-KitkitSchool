//
//  GameBoard.h on Aug 4, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Card.h"
#include "../Utils/MainNS.h"


BEGIN_NS_MOVINGINSECTS

class GameBoard: public Node {
    typedef Node Super;
    
    Label* LeftExprLabel;
    Label* OperatorLabel;
    Label* RightExprLabel;
    Label* EqualsLabel;
    Sprite* QuestionMarkSprite;
    
public:
    Chain<string> LeftExpr;
    Chain<string> Operator;
    Chain<string> RightExpr;

public:
    CREATE_FUNC(GameBoard);
    GameBoard();
    bool init() override;
    
    void showLeftExpr();
    void showOperator();
    void showRightExpr();
    void showEquals();
    void showQuestionMark();

    // NB(xenosoz, 2016): Move the card to this gameboard with some animation.
    //   The card should be "connceted" to world space before the function call.
    FiniteTimeAction* actionForPullCardToAnswerSlot(Card* C);
    void pullCardToAnswerSlot(Card* C);

public:
    static Size defaultSize();

private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_MOVINGINSECTS
