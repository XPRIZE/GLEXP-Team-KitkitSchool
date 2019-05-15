//
//  WoodenPuzzleStore.h -- Factory for GameBoard and PieceField
//  TodoSchool on Aug 18, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzleStore.h"
#include "../Utils/WoodenPuzzleUtil.h"


BEGIN_NS_WOODENPUZZLES;

void WoodenPuzzleStore::refinePlayCondition(GameBoard* Board, PlayField* Field) const {
    if (!Board || !Field) { return; }

    WoodenPuzzleUtil Util;
    vector<Node*> FlexibleNodes;
    Size CS = Field->getContentSize();
    
    for (auto Item : Field->WoodPieces) {
        FlexibleNodes.push_back(Item.second);
    }
    
    Rect BoardBB = Board->getBoundingBox();
    
    for (int I = 0; I < 5; ++I) {
        Util.refineNodePositions(FlexibleNodes, BoardBB, CS);
    }
}

END_NS_WOODENPUZZLES;
