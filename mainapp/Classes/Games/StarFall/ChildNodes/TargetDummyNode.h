//
//  TargetDummyNode.h on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/StarFallNS.h"


BEGIN_NS_STARFALL

class TargetDummyNode: public Node {
    typedef Node Super;
    Label* TextLabel;
    vector<Sprite*> Bubbles;

public:
    Chain<string> TitleText;
    
    function<void(TargetDummyNode&)> OnActionDidFinish;
    
public:
    CREATE_FUNC(TargetDummyNode);
    TargetDummyNode();
    bool init() override;

public:
    static Size defaultSize();
    
private:
    void clearInternals();
    void refreshChildNodes();
    
};

END_NS_STARFALL
