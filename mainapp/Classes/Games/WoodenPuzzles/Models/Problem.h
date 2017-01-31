//
//  Problem.h -- A problem for WoodenPuzzles
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/WoodenPuzzleNS.h"


BEGIN_NS_WOODENPUZZLES

class Problem {
public:
    string Motif;
    
    // NB(xenosoz, 2016): Position of a slot for ANCHOR_MIDDLE in the game board's coordinate.
    Point Position;

    string FaceSkin;
    string DepthSkin;
    string ShadowSkin;
    string SlotSkin;

public:
    Problem();
    Problem(const string& Motif,
            Point Position,
            const string& FaceSkin,
            const string& DepthSkin,
            const string& ShadowSkin,
            const string& SlotSkin);
    
public:
    static Problem fromInputStream(istream& IS);
    friend istream& operator>>(istream& IS, Problem& P);
};

END_NS_WOODENPUZZLES