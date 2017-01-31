//
//  RedDot.h -- A red dot
//  TodoSchool on Nov 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_DIGITALQUIZ

class RedDot: public Node {
    typedef Node Super;

public:
    CREATE_FUNC(RedDot);
    RedDot();
    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_DIGITALQUIZ
