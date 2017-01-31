//
//  TraceScrollView.h -- Scroller for the case when the string is too long to be presented
//  TodoSchool on May 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../LiveModels/TraceString.h"
#include "../LiveModels/TraceIndex.h"
#include "../StyleModels/TraceFieldStyle.h"
#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceScrollView: public ScrollView {
    typedef ScrollView Super;

public:
    Optional<size_t> TheScrollCache;
    Point ScrollStartPosition;
    Point ScrollStopPosition;
    float ScrollClock;

    Chain<TraceString> TheTraceString;
    Chain<TraceIndex> PassedIndex;
    Chain<TraceFieldStyle> TheStyle;
    Chain<bool> ClippingEnabled;
    Chain<bool> UseTightBoundingBox;

public:
    CREATE_FUNC(TraceScrollView);
    bool init() override;

private:
    void clearInternals();

    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;
    
    void scrollThisToZero();
    void scrollThisToBegin();
    void scrollThisToEnd();
    void scrollThisToCenter();

    void scrollTheGlyphToCenter();
    void scrollTheGlyphToRegion();
    
    void finishTheScrollNow();
};

END_NS_TRACEFIELD
