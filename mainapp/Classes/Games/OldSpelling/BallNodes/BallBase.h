//
//  BallBase.h -- A ball with a spell on it
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/SpellingNS.h"


BEGIN_NS_SPELLING;

class BallBase: public Node {
    typedef Node Super;
    
public:
    Chain<string> BallClass;
    
public:
    CREATE_FUNC(BallBase);
    bool init() override;
    
protected:
    virtual void clearInternals();
    virtual void refreshChildNodes();
    virtual void refreshChildComponents();
};


END_NS_SPELLING;