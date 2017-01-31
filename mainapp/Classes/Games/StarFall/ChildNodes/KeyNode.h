//
//  KeyNode.h on Jul 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/StarFallNS.h"


BEGIN_NS_STARFALL

class KeyNode: public cocos2d::Node {
    typedef Node Super;
    Button* SymbolButton;

public:
    Chain<string> Symbol;
    Chain<bool> Enabled;
    Chain<bool> TouchEnabled;

    function<void(KeyNode&)> OnKeyPressed;

public:
    CREATE_FUNC(KeyNode);
    KeyNode();
    bool init() override;
    
public:
    static Size defaultSize();
    
private:
    void clearInternals();
    void refreshChildNodes();
    
    bool special() const;
    void updateTitleColor();
};

END_NS_STARFALL
