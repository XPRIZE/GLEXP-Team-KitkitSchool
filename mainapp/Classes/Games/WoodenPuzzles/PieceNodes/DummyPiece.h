//
//  DummyPiece.h on Aug 30, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "PieceBase.h"


BEGIN_NS_WOODENPUZZLES

class DummyPiece: public PieceBase {
    typedef PieceBase Super;

public:
    CREATE_FUNC(DummyPiece);
    bool init() override;
    
    void switchTo2D();
    void switchTo3D();
    
protected:
    void clearInternals() override;
    void refreshComponents() override;
    void refreshChildNodes() override;
};


END_NS_WOODENPUZZLES
