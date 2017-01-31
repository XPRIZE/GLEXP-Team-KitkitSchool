//
//  Platform.h -- A stone platform with many answer balls
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../BallNodes/BallTraits.h"
#include "../Utils/MainNS.h"


BEGIN_NS_SPELLING

class Platform: public Node {
    typedef Node Super;
    BallSize TheBallSize;
    
    // NB(xenosoz, 2016): Weak references to answer balls, shadows
    vector<Sprite*> Shadows;
    
public:
    Chain<vector<string>> Letters;
    Chain<Point> LocalPosition;
    Chain<GLubyte> Opacity;

public:
    CREATE_FUNC(Platform);
    bool init(BallSize BS);

    size_t maxCapacity() const;
    size_t size() const;

    Point positionForSlotInWorldSpace(size_t SlotIndex) const;
    Point positionForSlotInLocalSpace(size_t SlotIndex) const;
    void hideShadowForBallID(size_t SlotIndex) const;
    void showShadowForBallID(size_t SlotIndex) const;
    
    FiniteTimeAction* actionForEnter(function<void()> Callback = nullptr);
    FiniteTimeAction* actionForExit(function<void()> Callback = nullptr);
    
    using Super::setPosition;
    void setPosition(float X, float Y) override;
    void setOpacity(GLubyte Opacity) override;

private:
    void clearInternals();
    void refreshChildNodes();
    
    bool init() override { assert(false); return false; }

};

END_NS_SPELLING