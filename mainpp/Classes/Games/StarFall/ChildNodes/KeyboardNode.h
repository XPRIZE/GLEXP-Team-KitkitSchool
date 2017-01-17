//
//  KeyboardNode.h on Jul 25, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "KeyNode.h"
#include "../Models/Symbol.h"
#include "../Utils/StarFallNS.h"


BEGIN_NS_STARFALL

class KeyboardNode: public Node {
    typedef Node Super;
    map<std::string, KeyNode*> SymbolToKeyNode;

public:
    Chain<SymbolLayoutType> SymbolLayout;
    Chain<SymbolSetType> EnabledSymbols;
    
    function<void(KeyboardNode&, string Symbol)> OnKeyPressed;

public:
    CREATE_FUNC(KeyboardNode);
    KeyboardNode();
    bool init() override;

private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_STARFALL