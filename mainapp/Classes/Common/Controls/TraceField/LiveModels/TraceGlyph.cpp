//
//  TraceGlyph.cpp -- A glyph consists of a sequence of TraceStrokes
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceGlyph.h"
#include "../Utils/TraceFieldDepot.h"
#include <Games/NumberTrace/Common/Basic/BigLabel.h>


BEGIN_NS_TRACEFIELD

void TraceGlyph::appendStroke(const TraceStroke& Stroke) {
    Strokes.push_back(Stroke);
}

void TraceGlyph::appendStroke(TraceStroke&& Stroke) {
    Strokes.push_back(std::move(Stroke));
}

void TraceGlyph::refreshBoundingBox(const Rect& RoughBB) {
    RoughBoundingBox = RoughBB;
    refreshTightBoundingBox();
}

const vector<TraceStroke>& TraceGlyph::strokes() const {
    return Strokes;
}

const Rect& TraceGlyph::tightBoundingBox() const {
    return TightBoundingBox;
}

const Rect& TraceGlyph::roughBoundingBox() const {
    return RoughBoundingBox;
}
    
void TraceGlyph::refreshTightBoundingBox() {
    bool IsFirstInput = true;

    for (const TraceStroke& S : Strokes) {
        Rect BB = S.boundingBox();
        TightBoundingBox = (IsFirstInput ? BB : TightBoundingBox.union_(BB));

        IsFirstInput = false;
    }
}

END_NS_TRACEFIELD