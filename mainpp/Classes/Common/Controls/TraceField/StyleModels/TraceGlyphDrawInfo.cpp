//
//  TraceGlyphDrawInfo.cpp -- Rendering information for tracing glyphs
//  TodoSchool on Jul 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceGlyphDrawInfo.h"


BEGIN_NS_TRACEFIELD

TraceGlyphDrawInfo::RangeType
TraceGlyphDrawInfo::rangeWithLineStyle(TraceLineStyle::Type T) const
{
    using LineType = TraceLineStyle::Type;

    switch (T) {
        case LineType::PassiveGuide:
            return RangeType(Index3, Index4);

        case LineType::PassiveStroke:
            return RangeType(Index0, Index1);

        case LineType::ActiveGuide:
            return RangeType(Index2, Index3);

        case LineType::ActiveStroke:
            return RangeType(Index1, Index2);

        default:
            CCLOG("No matching LineType in TraceGlyphDrawInfo::rangeWithLineStyle.");
    }

    return RangeType(TraceIndex(), TraceIndex());
}

bool TraceGlyphDrawInfo::operator==(const TraceGlyphDrawInfo& R) const {
    const TraceGlyphDrawInfo& L = *this;
    return (L.Index0 == R.Index0 &&
            L.Index1 == R.Index1 &&
            L.Index2 == R.Index2 &&
            L.Index3 == R.Index3 &&
            L.Index4 == R.Index4);
}

END_NS_TRACEFIELD
