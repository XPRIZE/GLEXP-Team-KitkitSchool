//
//  TraceGlyph.h -- A glyph consists of a sequence of TraceStrokes
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Common/Controls/TraceField/LiveModels/TraceStroke.h>
#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceGlyph {
    vector<TraceStroke> Strokes;
    
    // NB(xenosoz, 2016): Bounding box with points in TraceStroke.
    Rect TightBoundingBox;
    
    // NB(xenosoz, 2016): Bounding box with the size of Label.
    Rect RoughBoundingBox;

public:
    void appendStroke(const TraceStroke& Stroke);
    void appendStroke(TraceStroke&& Stroke);
    void refreshBoundingBox(const Rect& RoughBB);
    
    const vector<TraceStroke>& strokes() const;
    const Rect& tightBoundingBox() const;
    const Rect& roughBoundingBox() const;

private:
    void refreshTightBoundingBox();
};

END_NS_TRACEFIELD