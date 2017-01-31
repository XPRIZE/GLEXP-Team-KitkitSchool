//
//  TraceGlyphDrawInfo.h -- Rendering information for tracing glyphs
//  TodoSchool on Jul 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "TraceFieldStyle.h"
#include "../LiveModels/TraceIndex.h"
#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceGlyphDrawInfo {
public:
    typedef pair<TraceIndex, TraceIndex> RangeType;

    // NB(xenosoz, 2016): In ascending z-order for in the usual case:
    //   [I3, I4): Strokes that we never visited yet. (PassiveGuide)
    //   [I0, I1): Strokes that we already cleared. (PassiveStroke)
    //   [I2, I3): Guided region in the current stroke. (ActiveGuide)
    //   [I1, I2): Strokes that we already cleared. (ActiveStroke)
    
    TraceIndex Index0;
    TraceIndex Index1;
    TraceIndex Index2;
    TraceIndex Index3;
    TraceIndex Index4;
    
public:
    RangeType rangeWithLineStyle(TraceLineStyle::Type T) const;
    
    bool operator==(const TraceGlyphDrawInfo& R) const;
};

END_NS_TRACEFIELD
