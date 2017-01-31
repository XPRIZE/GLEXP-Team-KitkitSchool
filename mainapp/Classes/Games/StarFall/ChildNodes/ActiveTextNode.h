//
//  ActiveTextNode.h on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/StarFallNS.h"


BEGIN_NS_STARFALL

class ActiveTextNode: public Node {
    typedef Node Super;
    Label* ActiveTextLabel;
    Label* BadTextLabel;
    
public:
    Chain<string> ActiveText;
    Chain<string> BadText;
    
public:
    CREATE_FUNC(ActiveTextNode);
    ActiveTextNode();
    bool init() override;
    
private:
    void clearInternals();
    void refreshChildNodes();
    
    void showActiveText();
    void showBadText();
    void hideActiveText();
    void hideBadText();
};

END_NS_STARFALL