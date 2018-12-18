//
//  TraceGlyph.h -- A glyph consists of a sequence of TraceStrokes
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Common/Controls/TraceField/LiveModels/TraceStroke.h>
#include "../Utils/TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceGlyph {
    vector<TraceStroke> Strokes;
    
    // NB(xenosoz, 2016): Bounding box with points in TraceStroke.
    AARect TightBoundingBox;
    
    // NB(xenosoz, 2016): Bounding box with the size of Label.
    AARect RoughBoundingBox;

public:
    void appendStroke(const TraceStroke& Stroke);
    void appendStroke(TraceStroke&& Stroke);
    void refreshBoundingBox(const AARect& RoughBB);
    
    const vector<TraceStroke>& strokes() const;
    const AARect& tightBoundingBox() const;
    const AARect& roughBoundingBox() const;

private:
    void refreshTightBoundingBox();
};

END_NS_TRACEFIELD
