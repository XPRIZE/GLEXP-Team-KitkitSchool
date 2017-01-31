//
//  BallSlot.h -- Empty/full slot for SpellBall
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../BallNodes/DummyBall.h"
#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class BallSlot: public Node {
    typedef Node Super;
    
    DummyBall* TheDummyBall;
    Chain<bool> Filled;

public:
    // NB(xenosoz, 2016): There may be multiple slots with the same slot-class.
    Chain<string> SlotClass;

public:
    CREATE_FUNC(BallSlot);
    BallSlot();
    bool init() override;

    bool filled();
    void clearSlot();
    void fillSlot();
    
public:
    static Size defaultSize();
    
private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_SPELLING;