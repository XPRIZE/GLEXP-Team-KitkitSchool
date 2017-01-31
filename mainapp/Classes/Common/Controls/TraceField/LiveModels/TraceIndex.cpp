//
//  TraceIndex.cpp -- A pseudo iterator for LiveModels/Trace(Stroke|Glyph|String).
//  TodoSchool on Apr 26, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceIndex.h"
#include "TraceString.h"
#include "TraceGlyph.h"
#include "TraceStroke.h"


BEGIN_NS_TRACEFIELD

TraceIndex::TraceIndex()
    : TraceIndex(0) {}

TraceIndex::TraceIndex(size_t PointIndex)
    : TraceIndex(0, PointIndex) {}

TraceIndex::TraceIndex(size_t StrokeIndex, size_t PointIndex)
    : TraceIndex(0, StrokeIndex, PointIndex) {}

TraceIndex::TraceIndex(size_t GlyphIndex, size_t StrokeIndex, size_t PointIndex)
    : TraceIndex(0, GlyphIndex, StrokeIndex, PointIndex) {}

TraceIndex::TraceIndex(size_t StringIndex, size_t GlyphIndex, size_t StrokeIndex, size_t PointIndex)
    : TraceIndex(0, StringIndex, GlyphIndex, StrokeIndex, PointIndex) {}

TraceIndex::TraceIndex(size_t WorldIndex, size_t StringIndex, size_t GlyphIndex, size_t StrokeIndex, size_t PointIndex)
    : WorldIndex(WorldIndex)
    , StringIndex(StringIndex), GlyphIndex(GlyphIndex), StrokeIndex(StrokeIndex), PointIndex(PointIndex) {}


TraceIndex TraceIndex::begin() {
    return TraceIndex(0, 0, 0, 0, 0);
}

TraceIndex TraceIndex::end() {
    return TraceIndex(1, 0, 0, 0, 0);
}


string TraceIndex::__repr__() {
    stringstream ss;
    ss << "todoschool::TraceIndex(";
    ss << "WorldIndex: " << WorldIndex << ", ";
    ss << "StringIndex: " << StringIndex << ", ";
    ss << "GlyphIndex: " << GlyphIndex << ", ";
    ss << "StrokeIndex: " << StrokeIndex << ", ";
    ss << "PointIndex: " << PointIndex << ")";
    return ss.str();
}

//
// NB(xenosoz, 2016): Increment the index by N.
//   We're actually traversing TraceString (or Glyph, Stroke) tree here.
//   To make it efficient, please consider putting weight for each node in the tree.
//
TraceIndex& TraceIndex::increment(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke, size_t N)
{
    while (WorldIndex < 1)
    {
        // NB(xenosoz, 2016): string -> glyph -> stroke -> point
        size_t StringCount = (String ? 1 : 0);
        if (StringIndex >= StringCount) {
            // NB(xenosoz, 2016): Special case hitting the fixpoint (ceiling).
            WorldIndex = 1;
            StringIndex = 0;
            GlyphIndex = 0;
            StrokeIndex = 0;
            PointIndex = 0;
            N = 0;
            break;
        }

        size_t GlyphCount = (String ? String->size() : 0);
        if (GlyphIndex >= GlyphCount) {
            StringIndex += 1;
            GlyphIndex = 0;
            StrokeIndex = 0;
            PointIndex = 0;
            continue;
        }

        Glyph = (String ? &(String->at(GlyphIndex)) : Glyph);
        size_t StrokeCount = (Glyph ? Glyph->strokes().size() : 0);
        if (StrokeIndex >= StrokeCount) {
            GlyphIndex += 1;
            StrokeIndex = 0;
            PointIndex = 0;
            continue;
        }

        Stroke = (Glyph ? &(Glyph->strokes()[StrokeIndex]) : Stroke);
        size_t PointCount = (Stroke ? Stroke->samplePoints().size() : 0);

        size_t PointDeltaToStrokeEnd = PointCount - PointIndex;
        if (N >= PointDeltaToStrokeEnd) {
            StrokeIndex += 1;
            PointIndex = 0;
            N -= PointDeltaToStrokeEnd;
            continue;
        }

        PointIndex += N;
        N = 0;
        break;
    }
    return *this;
}

void TraceIndex::sanitize(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke) {
    if (WorldIndex > 0) {
        StringIndex = 0;
        GlyphIndex = 0;
        StrokeIndex = 0;
        return;
    }
    this->increment(String, Glyph, Stroke, 0);
}

TraceIndex TraceIndex::indexForGlyphBegin(const TraceString* String, const TraceGlyph* Glyph) const {
    TraceIndex It = *this;
    It.StrokeIndex = 0;
    It.PointIndex = 0;
    It.sanitize(String, Glyph, nullptr);
    return It;
}

TraceIndex TraceIndex::indexForGlyphEnd(const TraceString* String, const TraceGlyph* Glyph) const {
    TraceIndex It = *this;
    It.GlyphIndex += 1;
    It.StrokeIndex = 0;
    It.PointIndex = 0;
    It.sanitize(String, Glyph, nullptr);
    return It;
}

TraceIndex TraceIndex::indexForStrokeBegin(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke) const {
    TraceIndex It = *this;
    It.PointIndex = 0;
    It.sanitize(String, Glyph, Stroke);
    return It;
}

TraceIndex TraceIndex::indexForStrokeEnd(const TraceString* String, const TraceGlyph* Glyph, const TraceStroke* Stroke) const {
    TraceIndex It = *this;
    It.StrokeIndex += 1;
    It.PointIndex = 0;
    It.sanitize(String, Glyph, Stroke);
    return It;
}

TraceIndex& TraceIndex::increment(const TraceString& String, size_t N) {
    return this->increment(&String, nullptr, nullptr, N);
}

TraceIndex& TraceIndex::increment(const TraceGlyph& Glyph, size_t N) {
    return this->increment(nullptr, &Glyph, nullptr, N);
}

TraceIndex& TraceIndex::increment(const TraceStroke& Stroke, size_t N) {
    return this->increment(nullptr, nullptr, &Stroke, N);
}

void TraceIndex::sanitize(const TraceString& String) {
    this->sanitize(&String, nullptr, nullptr);
}

void TraceIndex::sanitize(const TraceGlyph& Glyph) {
    this->sanitize(nullptr, &Glyph, nullptr);
}

void TraceIndex::sanitize(const TraceStroke& Stroke) {
    this->sanitize(nullptr, nullptr, &Stroke);
}

TraceIndex TraceIndex::indexForGlyphBegin(const TraceString& String) const {
    return this->indexForGlyphBegin(&String, nullptr);
}

TraceIndex TraceIndex::indexForGlyphBegin(const TraceGlyph& Glyph) const {
    return this->indexForGlyphBegin(nullptr, &Glyph);
}

TraceIndex TraceIndex::indexForGlyphEnd(const TraceString& String) const {
    return this->indexForGlyphEnd(&String, nullptr);
}

TraceIndex TraceIndex::indexForGlyphEnd(const TraceGlyph& Glyph) const {
    return this->indexForGlyphEnd(nullptr, &Glyph);
}

TraceIndex TraceIndex::indexForStrokeBegin(const TraceString& String) const {
    return this->indexForStrokeBegin(&String, nullptr, nullptr);
}

TraceIndex TraceIndex::indexForStrokeBegin(const TraceGlyph& Glyph) const {
    return this->indexForStrokeBegin(nullptr, &Glyph, nullptr);
}

TraceIndex TraceIndex::indexForStrokeBegin(const TraceStroke& Stroke) const {
    return this->indexForStrokeBegin(nullptr, nullptr, &Stroke);
}

TraceIndex TraceIndex::indexForStrokeEnd(const TraceString& String) const {
    return this->indexForStrokeEnd(&String, nullptr, nullptr);
}

TraceIndex TraceIndex::indexForStrokeEnd(const TraceGlyph& Glyph) const {
    return this->indexForStrokeEnd(nullptr, &Glyph, nullptr);
}

TraceIndex TraceIndex::indexForStrokeEnd(const TraceStroke& Stroke) const {
    return this->indexForStrokeEnd(nullptr, nullptr, &Stroke);
}

TraceIndex TraceIndex::clip(const TraceIndex& Begin, const TraceIndex End) const {
    return min(max(Begin, *this), End);
}

const TracePoint& TraceIndex::pointFor(const TraceString& String) const {
    if (WorldIndex > 0 || StringIndex > 0)
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    if (GlyphIndex >= String.size())
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    auto& Glyph = String[GlyphIndex];
    if (StrokeIndex >= Glyph.strokes().size())
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    auto& Stroke = Glyph.strokes()[StrokeIndex];
    if (PointIndex >= Stroke.samplePoints().size())
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    auto& Point = Stroke.samplePoints()[PointIndex];

    return Point;
}

const TracePoint& TraceIndex::pointFor(const TraceGlyph& Glyph) const {
    if (WorldIndex > 0 || StringIndex > 0 || GlyphIndex > 0)
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    if (StrokeIndex >= Glyph.strokes().size())
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    auto& Stroke = Glyph.strokes()[StrokeIndex];
    if (PointIndex >= Stroke.samplePoints().size())
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    auto& Point = Stroke.samplePoints()[PointIndex];

    return Point;
}

const TracePoint& TraceIndex::pointFor(const TraceStroke& Stroke) const {
    if (WorldIndex > 0 || StringIndex > 0 || GlyphIndex > 0 || StrokeIndex > 0)
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    if (PointIndex >= Stroke.samplePoints().size())
        throw std::out_of_range("cocos2d::TraceIndex in TraceIndex::PointFor");

    auto& Point = Stroke.samplePoints()[PointIndex];
    return Point;
}


bool TraceIndex::operator<(const TraceIndex& RHS) const {
    // NB(xenosoz, 2016): The one operator to rule them all.
    const TraceIndex& LHS = *this;

    if (LHS.WorldIndex != RHS.WorldIndex)
        return LHS.WorldIndex < RHS.WorldIndex;

    if (LHS.StringIndex != RHS.StringIndex)
        return LHS.StringIndex < RHS.StringIndex;

    if (LHS.GlyphIndex != RHS.GlyphIndex)
        return LHS.GlyphIndex < RHS.GlyphIndex;

    if (LHS.StrokeIndex != RHS.StrokeIndex)
        return LHS.StrokeIndex < RHS.StrokeIndex;

    return LHS.PointIndex < RHS.PointIndex;
}

bool TraceIndex::operator<=(const TraceIndex& RHS) const {
    // (LHS <= RHS) == !(LHS > RHS) == !(RHS < LHS)
    const TraceIndex& LHS = *this;
    return !(RHS < LHS);
}

bool TraceIndex::operator>(const TraceIndex& RHS) const {
    // (LHS > RHS) == (RHS < LHS)
    const TraceIndex& LHS = *this;
    return RHS < LHS;
}

bool TraceIndex::operator>=(const TraceIndex& RHS) const {
    // (LHS >= RHS) == !(LHS < RHS)
    const TraceIndex& LHS = *this;
    return !(LHS < RHS);
}

bool TraceIndex::operator==(const TraceIndex& RHS) const {
    // (LHS == RHS) == !(LHS < RHS) && !(LHS > RHS) == !(LHS < RHS) && !(RHS < LHS)
    const TraceIndex& LHS = *this;
    return !(LHS < RHS) && !(RHS < LHS);
}

bool TraceIndex::operator!=(const TraceIndex& RHS) const {
    // (LHS != RHS) == (LHS < RHS) || (LHS > RHS) == (LHS < RHS) || (RHS < LHS)
    const TraceIndex& LHS = *this;
    return (LHS < RHS) || (RHS < LHS);
}

END_NS_TRACEFIELD