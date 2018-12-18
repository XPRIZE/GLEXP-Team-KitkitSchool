//
//  WoodPieceShadow.h -- Shadow of WoodPiece
//  TodoSchool on Aug 19, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

// NB(xenosoz, 2016): The shadow is darker when it's not on the GameBoard so we implement the shadow node separately (again).


#pragma once

#include "../Utils/WoodenPuzzleNS.h"
#include <Common/Basic/CreateFunc.h>


BEGIN_NS_WOODENPUZZLES

class WoodPieceShadow: public Node {
    typedef Node Super;

public:
    Chain<string> PieceID;
    Chain<Point> PositionInWorldSpace;

public:
    CREATE_FUNC(WoodPieceShadow);
    bool init(const string& Mode);
    
    string Mode;
    
private:
    void clearInternals();
    void refreshChildNodes();
};

END_NS_WOODENPUZZLES
