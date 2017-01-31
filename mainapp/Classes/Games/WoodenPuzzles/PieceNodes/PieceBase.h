//
//  PieceBase.h on Aug 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/WoodenPuzzleNS.h"
#include <Common/Basic/SoundEffect.h>


BEGIN_NS_WOODENPUZZLES;


class PieceBase: public Node {
    typedef Node Super;

protected:
    Node* FaceNode;
    Node* DepthNode;
    Node* ShadowNode;
    
public:
    Chain<string> PieceID;
    Chain<string> FaceSkin;
    Chain<string> DepthSkin;
    Chain<string> ShadowSkin;
    
public:
    CREATE_FUNC(PieceBase);
    PieceBase();
    bool init() override;
    
protected:
    virtual void clearInternals();
    virtual void refreshComponents();
    virtual void refreshChildNodes();
};


END_NS_WOODENPUZZLES;
