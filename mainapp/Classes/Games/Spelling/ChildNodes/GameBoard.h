//
//  GameBoard.h -- Game board with a picture and answer slots on it
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "BallSlot.h"
#include "../Models/Problem.h"
#include "../Utils/MainNS.h"


BEGIN_NS_SPELLING

class GameBoard: public Node {
    typedef Node Super;
    BallSize TheBallSize;
    SoundEffect SoundForEnter;
    
    // NB(xenosoz, 2016): Weak references for slots
    vector<BallSlot*> Slots;
    
public:
    Chain<Problem> TheProblem;
    
    function<void()> OnAllSlotDidFill;
    
public:
    CREATE_FUNC(GameBoard);
    bool init() override;
    bool init(BallSize BS);
    
    size_t size() const;
    BallSlot* slotAtIndex(size_t Index) const;
    
    FiniteTimeAction* actionForEnter(function<void()> Callback = nullptr);
    FiniteTimeAction* actionForEjectBalls(function<void()> Callback = nullptr);
    FiniteTimeAction* actionForExit(function<void()> Callback = nullptr);


private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_SPELLING
