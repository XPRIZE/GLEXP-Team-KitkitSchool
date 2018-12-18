//
//  WoodSlot.h on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../PieceNodes/DummyPiece.h"
#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES;


class WoodSlot: public Node {
    typedef Node Super;

    Sprite* SlotSprite;
    DummyPiece* TheDummyPiece;
    
public:
    Chain<string> Mode;
    Chain<string> SlotID;
    Chain<string> FaceSkin;
    Chain<string> DepthSkin;
    Chain<string> ShadowSkin;
    Chain<string> SlotSkin;

public:
    CREATE_FUNC(WoodSlot);
    WoodSlot();
    bool init() override;
    
    void clearSlot();
    void fillSlot();
    void pullPieceIllusion(Point PositionInWorldSpace);
    
private:
    void clearInternals();
    void refreshChildNodes();
};


END_NS_WOODENPUZZLES;
