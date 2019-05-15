//
//  Problem.cpp -- A problem for WoodenPuzzles
//  TodoSchool on Oct 15, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzlesProblem.h"


BEGIN_NS_WOODENPUZZLES

Problem::Problem() {
}

Problem::Problem(const string& Motif,
                 Point Position,
                 const string& FaceSkin,
                 const string& DepthSkin,
                 const string& ShadowSkin,
                 const string& SlotSkin)
: Motif(Motif)
, Position(Position)
, FaceSkin(FaceSkin)
, DepthSkin(DepthSkin)
, ShadowSkin(ShadowSkin)
, SlotSkin(SlotSkin)
{
}

Problem Problem::fromInputStream(istream& IS) {
    string Motif;
    Point Pos;
    string Face, Depth, Shadow, Slot;
    
    if (IS >> Motif >> Pos.x >> Pos.y >> Face >> Depth >> Shadow >> Slot) {
        return Problem(Motif, Pos, Face, Depth, Shadow, Slot);
    }

    return Problem();
}

istream& operator>>(istream& IS, Problem& P) {
    P = Problem::fromInputStream(IS);
    return IS;
}


END_NS_WOODENPUZZLES
