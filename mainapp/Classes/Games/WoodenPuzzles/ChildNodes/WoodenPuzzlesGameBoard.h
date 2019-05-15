//
//  GameBoard.h on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "WoodSlot.h"
#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES;


class GameBoard: public Node {
    typedef Node Super;
    
public:
    map<string, WoodSlot*> WoodSlots;

    Chain<string> ContentSkinPath;

public:
    CREATE_FUNC(GameBoard);
    bool init() override;
    
    void appendWoodSlot(WoodSlot* Slot);
    void promoteWoodSlot(WoodSlot* Slot);
    
private:
    void clearInternals();
    void refreshChildNodes();
};


END_NS_WOODENPUZZLES;
