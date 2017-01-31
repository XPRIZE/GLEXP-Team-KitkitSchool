//
//  AnswerPad.h -- Empty/full slot for SpellBall
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "BallSlot.h"
#include "../Models/Work.h"
#include "../BallNodes/SpellBall.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class AnswerPad: public Node {
    typedef Node Super;

    vector<BallSlot*> Slots;

public:
    Chain<Work> TheWork;
    
    function<void()> OnAnswerDidComplete;
    
public:
    CREATE_FUNC(AnswerPad);
    bool init() override;

    size_t size() const;
    Point slotPositionWithIndex(size_t Index) const;
    void fillSlotWithIndex(size_t Index);

    void tryToCatchBall(SpellBall* Ball);


private:
    void clearInternals();
    void refreshChildNodes();
    
    void checkForCompletion();
};

END_NS_SPELLING;

