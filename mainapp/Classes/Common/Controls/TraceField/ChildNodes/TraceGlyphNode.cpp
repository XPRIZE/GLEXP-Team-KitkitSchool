//
//  TraceGlyphNode.cpp -- Display a n-th glyph given by Index
//  TodoSchool on Apr 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceGlyphNode.h"
#include "../LiveModels/TraceStroke.h"
#include "../LiveModels/TraceGlyph.h"
#include "../LiveModels/TraceString.h"


BEGIN_NS_TRACEFIELD

bool TraceGlyphNode::init() {
    if (!Super::init()) { return false; }
    
    HasDrawCache = false;
    clearInternals();

    return true;
}


void TraceGlyphNode::clearInternals() {
    BaseIndex.OnValueUpdate = [this](TraceIndex&) {
        BeginIndex = BaseIndex().indexForGlyphBegin(TheTraceString());
        EndIndex = BaseIndex().indexForGlyphEnd(TheTraceString());
        
        TheDrawCache = TraceGlyphDrawInfo();
        HasDrawCache = false;
    };
    
    TheTraceString.OnValueUpdate = [this](TraceString&) {
        BeginIndex = BaseIndex().indexForGlyphBegin(TheTraceString());
        EndIndex = BaseIndex().indexForGlyphEnd(TheTraceString());
        
        TheDrawCache = TraceGlyphDrawInfo();
        HasDrawCache = false;
    };
}

void TraceGlyphNode::onEnter() {
    Super::onEnter();
    scheduleUpdate();
}

void TraceGlyphNode::onExit() {
    Super::onExit();
    unscheduleUpdate();
}

void TraceGlyphNode::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);

    const TraceString& String = TheTraceString();

    // NB(xenosoz, 2016): Always make sure that boundaries below is valid in the given String space.
    TraceGlyphDrawInfo DI;
    DI.Index0 = BeginIndex;
    DI.Index1 = clipIndex(PassedIndex().indexForStrokeBegin(String));
    DI.Index2 = clipIndex(PassedIndex());
    DI.Index3 = clipIndex(PassedIndex().indexForStrokeEnd(String));
    DI.Index4 = EndIndex;

    if (HasDrawCache && TheDrawCache == DI) {
        // NB(xenosoz, 2016): We're clean now. Nothing to draw yet.
        return;
    }

    Super::clear();
    for (auto& S : TheStyle().LineStyles) {
        TraceIndex StartIndex, StopIndex;
        std::tie(StartIndex, StopIndex) = DI.rangeWithLineStyle(S.TheType);
        
        drawTraceStringInRange(StartIndex, StopIndex, S.Thickness / getScale(), S.Color);
    }
    
    // Set it clean.
    TheDrawCache = DI;
    HasDrawCache = false;
}

void TraceGlyphNode::drawTraceStringInRange(const TraceIndex& Start, const TraceIndex Stop,
                                            float Thickness, cocos2d::Color4B Color)
{
    const TraceString& String = TheTraceString();

    typedef vector<Point> Stroke;
    typedef vector<Stroke> Strokes;
    
    Strokes SS;
    SS.push_back(Stroke());
    
    for (TraceIndex I = Start, J = Start, E = Stop; J != E; I = J, J.increment(String)) {
        if (I.indexForStrokeBegin(String) != J.indexForStrokeBegin(String)) {
            SS.push_back(Stroke());
            I = J;
        }
        TracePoint PI = I.pointFor(String);
        TracePoint PJ = J.pointFor(String);
        
        SS.back().push_back(PI.Position);
        SS.back().push_back(PJ.Position);
    }
    
    for (auto& S : SS) {
        if (S.size() == 0) { continue; }
        drawSegments(&S[0], (int)S.size(), Thickness, Color);
    }
}

TraceIndex TraceGlyphNode::clipIndex(const TraceIndex& Index) {
    return Index.clip(BeginIndex, EndIndex);
}

END_NS_TRACEFIELD
