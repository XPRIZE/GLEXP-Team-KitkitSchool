//
//  TraceCursor.h -- The trace cursor with arrow sign
//  TodoSchool on Apr 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceCursor: public Sprite {
    typedef Sprite Super;

public:
    Chain<bool> Enabled;

    Chain<Point> CursorPointInGlyphNode;
    Chain<Vec2> CursorVelocity;
    Chain<bool> CursorPicked;

    Chain<float> NormalScale;
    Chain<float> PickedScale;

public:
    static TraceCursor* create();
    static TraceCursor* createWithFilename(const string& Filename);
    bool initWithFilename(const string& Filename);

private:
    void clearInternals();

    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;
};

END_NS_TRACEFIELD
