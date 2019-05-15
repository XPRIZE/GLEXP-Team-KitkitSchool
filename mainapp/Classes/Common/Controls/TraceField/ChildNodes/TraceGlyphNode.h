//
//  TraceGlyphNode.h -- Display a n-th glyph given by Index
//  TodoSchool on Apr 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../LiveModels/TraceString.h"
#include "../LiveModels/TraceIndex.h"
#include "../StyleModels/TraceFieldStyle.h"
#include "../StyleModels/TraceGlyphDrawInfo.h"
#include "../Utils/TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceGlyphNode: public TodoSchoolDrawNode {
    typedef DrawNode Super;

    // NB(xenosoz, 2016): Nullable. Nullptr always means we're dirty.
    bool HasDrawCache;
    TraceGlyphDrawInfo TheDrawCache;
    TraceIndex BeginIndex;
    TraceIndex EndIndex;

public:
    Chain<TraceString> TheTraceString;
    Chain<TraceIndex> BaseIndex;
    Chain<TraceIndex> PassedIndex;
    Chain<TraceFieldStyle> TheStyle;

public:
    CREATE_FUNC(TraceGlyphNode);
    bool init() override;

private:
    void clearInternals();

    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;

    void handleTraceStringValueUpdate(TraceString&);

private:
    void drawTraceStringInRange(const TraceIndex& Start, const TraceIndex Stop,
                                float Thickness, Color4B Color);
    TraceIndex clipIndex(const TraceIndex& Index);
};

END_NS_TRACEFIELD
