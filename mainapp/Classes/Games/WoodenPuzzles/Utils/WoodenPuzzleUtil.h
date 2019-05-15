//
//  WoodenPuzzleUtil.h on Jun 8, 2018
//  TodoSchool
//
//  Copyright (c) 2018 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "WoodenPuzzleNS.h"

BEGIN_NS_WOODENPUZZLES;


class WoodenPuzzleUtil {
public:
    Point wallBiasedRandomPoint(const Rect& Region) const;
    float costOfNodePositions(const vector<Node*>& FlexibleNodes,
                              Rect BoardBB) const;
    
    void refineNodePositions(const vector<Node*>& FlexibleNodes,
                             Rect BoardBB,
                             Size RegionSize) const;
};


END_NS_WOODENPUZZLES;
