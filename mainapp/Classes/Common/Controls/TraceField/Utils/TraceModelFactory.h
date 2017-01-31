//
//  TraceModelFactory.h -- (string -> TraceString) + (string -> TraceGlyph)
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../LiveModels/TraceString.h"
#include "../LiveModels/TraceGlyph.h"
#include "../LiveModels/TracePoint.h"
#include "../ArchiveModels/TraceKnotList.h"
#include "MainNS.h"


BEGIN_NS_TRACEFIELD

class TraceModelFactory {
public:
    TraceString stringWithString(const string& Str) const;
    TraceGlyph glyphWithString(const string& Str) const;
    TraceGlyph glyphWithKnotList(const TraceKnotList& KnotList, Rect RoughBB) const;
    
    TraceString resizeStringVertically(const TraceString& Source,
                                       Rect TargetRect,
                                       bool UseTightBB = false) const;
    TraceString resizeString(const TraceString& Source,
                             Rect TargetRect,
                             bool UseTightBB = false) const;
    
private:
    TraceString transformedString(const TraceString& SourceString,
                                  Vec2 PreTranslate,
                                  float Scale,
                                  Vec2 PostTranslate) const;
};

END_NS_TRACEFIELD
