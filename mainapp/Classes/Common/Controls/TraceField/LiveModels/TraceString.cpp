//
//  TraceString.cpp -- A string consists of a sequence of TraceGlyphs
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceString.h"


BEGIN_NS_TRACEFIELD

void TraceString::clear() {
    Glyphs.clear();
}

size_t TraceString::size() const {
    return Glyphs.size();
}

void TraceString::appendGlyph(const TraceGlyph& Glyph) {
    Glyphs.push_back(Glyph);
}

void TraceString::appendGlyph(TraceGlyph&& Glyph) {
    Glyphs.push_back(std::move(Glyph));
}

TraceString::iterator TraceString::begin() {
    return Glyphs.begin();
}

TraceString::const_iterator TraceString::begin() const {
    return Glyphs.begin();
}

TraceString::iterator TraceString::end() {
    return Glyphs.end();
}

TraceString::const_iterator TraceString::end() const {
    return Glyphs.end();
}

TraceGlyph& TraceString::at(size_t N) {
    return Glyphs.at(N);
}

const TraceGlyph& TraceString::at(size_t N) const {
    return Glyphs.at(N);
}

TraceGlyph& TraceString::operator[](size_t N) {
    return Glyphs[N];
}

const TraceGlyph& TraceString::operator[](size_t N) const {
    return Glyphs[N];
}

TraceGlyph& TraceString::front() {
    return Glyphs.front();
}

const TraceGlyph& TraceString::front() const {
    return Glyphs.front();
}

TraceGlyph& TraceString::back() {
    return Glyphs.back();
}

const TraceGlyph& TraceString::back() const {
    return Glyphs.back();
}

Point TraceString::offsetForGlyphAt(size_t N) const {
    return Offsets[N];
}
    
Point TraceString::centerForGlyphAt(size_t N) const {
    Rect BB = Glyphs[N].roughBoundingBox();
    return Offsets[N] + Point(BB.midX(), BB.midY());
}
    
Point TraceString::minPointForGlyphAt(size_t N) const {
    Rect BB = Glyphs[N].roughBoundingBox();
    return Offsets[N] + Point(BB.minX(), BB.minY());
}

Point TraceString::maxPointForGlyphAt(size_t N) const {
    Rect BB = Glyphs[N].roughBoundingBox();
    return Offsets[N] + Point(BB.maxX(), BB.maxY());
}
    
Rect TraceString::tightBoundingBoxForGlyphAt(size_t N) const {
    Rect BB = Glyphs[N].tightBoundingBox();
    BB.origin += Offsets[N];
    return BB;
}

Rect TraceString::roughBoundingBoxForGlyphAt(size_t N) const {
    Rect BB = Glyphs[N].roughBoundingBox();
    BB.origin += Offsets[N];
    return BB;
}

void TraceString::refreshBoundingBox() {
    Point Offset = Point::ZERO;
    Rect StringTightBB = (Glyphs.size() > 0 ? Glyphs[0].tightBoundingBox() : Rect::zero());
    Rect StringRoughBB = (Glyphs.size() > 0 ? Glyphs[0].roughBoundingBox() : Rect::zero());
    
    for (size_t GlyphIndex = 0; GlyphIndex < Glyphs.size(); ++GlyphIndex) {
        const float Spacing = 40.f;  // XXX
        Rect LocalRoughBB = Glyphs[GlyphIndex].roughBoundingBox();
        
        if (GlyphIndex > 0) {
            Offset.x += Spacing;
            Offset.x -= LocalRoughBB.minX();
        }

        Rect GlyphTightBB = Glyphs[GlyphIndex].tightBoundingBox();
        Rect GlyphRoughBB = Glyphs[GlyphIndex].roughBoundingBox();
        GlyphTightBB.origin += Offset;
        GlyphRoughBB.origin += Offset;
        StringTightBB = StringTightBB.union_(GlyphTightBB);
        StringRoughBB = StringRoughBB.union_(GlyphRoughBB);

        Offsets.push_back(Offset);
        Offset.x += LocalRoughBB.maxX();
    }
    
    TightBoundingBox = StringTightBB;
    RoughBoundingBox = StringRoughBB;
}

const Rect& TraceString::tightBoundingBox() const {
    return TightBoundingBox;
}

const Rect& TraceString::roughBoundingBox() const {
    return RoughBoundingBox;
}

END_NS_TRACEFIELD