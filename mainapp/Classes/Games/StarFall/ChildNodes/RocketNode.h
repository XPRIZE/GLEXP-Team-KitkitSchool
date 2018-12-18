//
//  RocketNode.h on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/StarFallNS.h"


BEGIN_NS_STARFALL

class RocketNode: public Node {
    typedef Node Super;

public:
    Chain<Point> BasePosition;
    Chain<Point> TargetPosition;
    
    function<void(RocketNode&)> OnActionDidFinish;

public:
    CREATE_FUNC(RocketNode);
    RocketNode();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_STARFALL
