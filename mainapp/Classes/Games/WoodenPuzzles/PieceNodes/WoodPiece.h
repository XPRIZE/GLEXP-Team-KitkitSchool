//
//  WoodPiece.h on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "PieceBase.h"


BEGIN_NS_WOODENPUZZLES;


class WoodPiece: public PieceBase {
    typedef PieceBase Super;

    RefPtr<Image> ShadowImage;

public:
    SoundEffect PieceNameSound;
    Chain<Point> PositionInWorldSpace;

    function<void(Touch*, Event*)> OnTouch;
    function<void(Touch*, Event*)> OnDrag;
    function<void(Touch*, Event*)> OnRelease;

public:
    CREATE_FUNC(WoodPiece);
    WoodPiece();
    bool init() override;
    
    void setPosition(const Point& P) override;
    void setPosition3D(const Vec3& P) override;  // NB(xenosoz, 2016): cocos2d::MoveTo calls setP3D, not setP.
    
protected:
    void clearInternals() override;
    void refreshComponents() override;
    void refreshChildNodes() override;
    
    bool hitTestForShadowImage(Touch* T, Event* E);
};


END_NS_WOODENPUZZLES;
