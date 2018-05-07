//
//  TraceIndex.h -- A pseudo iterator for LiveModels/Trace(Stroke|Glyph|String).
//  TodoSchool on Apr 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceString;
class TraceGlyph;
class TraceStroke;
class TracePoint;

class TraceIndex {
public:
    //
    // NB(xenosoz, 2016): WorldIndex is the topmost index
    //   which represents a concept similar to ::end() of STL containers.
    //
    //   (wo, st, gl, st, po) = (1, 0, 0, 0, 0)
    //                        | (0, _, _, _, _)
    //   where _ means don't care term.
    //
    size_t WorldIndex;  // 0 or 1. like from Heaven (valid region) to Hell (invalid region).
    size_t StringIndex;
    size_t GlyphIndex;
    size_t StrokeIndex;
    size_t PointIndex;

public:
    TraceIndex();
    TraceIndex(size_t PointIndex);
    TraceIndex(size_t StrokeIndex, size_t PointIndex);
    TraceIndex(size_t GlyphIndex, size_t StrokeIndex, size_t PointIndex);
    TraceIndex(size_t StringIndex, size_t GlyphIndex, size_t StrokeIndex, size_t PointIndex);
    TraceIndex(size_t WorldIndex, size_t StringIndex, size_t GlyphIndex, size_t StrokeIndex, size_t PointIndex);

    static TraceIndex begin();
    static TraceIndex end();

    string __repr__();

private:
    // NB(xenosoz, 2016): Only one of String, Glyph, Stroke should be non-nullptr.
    TraceIndex& increment(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke, size_t N);
    void sanitize(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke);
    TraceIndex indexForGlyphBegin(const TraceString* String, const TraceGlyph* Glyph) const;
    TraceIndex indexForGlyphEnd(const TraceString* String, const TraceGlyph* Glyph) const;
    TraceIndex indexForStrokeBegin(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke) const;
    TraceIndex indexForStrokeEnd(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke) const;

public:
    TraceIndex& increment(const TraceString& String, size_t N=1);
    TraceIndex& increment(const TraceGlyph& Glyph, size_t N=1);
    TraceIndex& increment(const TraceStroke& Stroke, size_t N=1);
    void sanitize(const TraceString& String);
    void sanitize(const TraceGlyph& Glyph);
    void sanitize(const TraceStroke& Stroke);
    TraceIndex indexForGlyphBegin(const TraceString& String) const;
    TraceIndex indexForGlyphBegin(const TraceGlyph& Glyph) const;
    TraceIndex indexForGlyphEnd(const TraceString& String) const;
    TraceIndex indexForGlyphEnd(const TraceGlyph& Glyph) const;
    TraceIndex indexForStrokeBegin(const TraceString& String) const;
    TraceIndex indexForStrokeBegin(const TraceGlyph& Glyph) const;
    TraceIndex indexForStrokeBegin(const TraceStroke& Stroke) const;
    TraceIndex indexForStrokeEnd(const TraceString& String) const;
    TraceIndex indexForStrokeEnd(const TraceGlyph& Glyph) const;
    TraceIndex indexForStrokeEnd(const TraceStroke& Stroke) const;

    TraceIndex clip(const TraceIndex& Begin, const TraceIndex End) const;

    const TracePoint& pointFor(const TraceString& String) const;
    const TracePoint& pointFor(const TraceGlyph& Glyph) const;
    const TracePoint& pointFor(const TraceStroke& Stroke) const;

public:
    bool operator<(const TraceIndex& RHS) const;
    bool operator<=(const TraceIndex& RHS) const;
    bool operator>(const TraceIndex& RHS) const;
    bool operator>=(const TraceIndex& RHS) const;
    bool operator==(const TraceIndex& RHS) const;
    bool operator!=(const TraceIndex& RHS) const;
};

END_NS_TRACEFIELD
