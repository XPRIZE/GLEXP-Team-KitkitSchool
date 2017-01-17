//
//  TraceString.h -- A string consists of a sequence of TraceGlyphs
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../LiveModels/TraceGlyph.h"
#include "../Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceString {
public:
    typedef vector<TraceGlyph>::iterator iterator;
    typedef vector<TraceGlyph>::const_iterator const_iterator;

private:
    vector<TraceGlyph> Glyphs;
    vector<Point> Offsets;

    // NB(xenosoz, 2016): Bounding box with points in TraceGlyph.
    Rect TightBoundingBox;
    
    // NB(xenosoz, 2016): Bounding box with the size of Label.
    Rect RoughBoundingBox;

public:
    void clear();
    size_t size() const;

    void appendGlyph(const TraceGlyph& Glyph);
    void appendGlyph(TraceGlyph&& Glyph);

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    
    TraceGlyph& at(size_t N);
    const TraceGlyph& at(size_t N) const;
    TraceGlyph& operator[](size_t N);
    const TraceGlyph& operator[](size_t N) const;
    
    TraceGlyph& front();
    const TraceGlyph& front() const;
    TraceGlyph& back();
    const TraceGlyph& back() const;
    
    Point offsetForGlyphAt(size_t N) const;
    Point centerForGlyphAt(size_t N) const;
    Point minPointForGlyphAt(size_t N) const;
    Point maxPointForGlyphAt(size_t N) const;
    Rect tightBoundingBoxForGlyphAt(size_t N) const;
    Rect roughBoundingBoxForGlyphAt(size_t N) const;

    void refreshBoundingBox();
    const Rect& tightBoundingBox() const;
    const Rect& roughBoundingBox() const;
};

END_NS_TRACEFIELD
