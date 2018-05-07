//
//  TraceModelFactory.h -- (string -> TraceString) + (string -> TraceGlyph)
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//  which is again from smoothPointsAndNormals() in TracingGuideline.m (Locomotive Labs)
//


#include "TraceModelFactory.h"
#include "TraceFieldDepot.h"
#include "../LiveModels/TracePoint.h"
#include "../LiveModels/TraceStroke.h"
#include "../ArchiveModels/TraceKnotPoint.h"
#include "Common/Basic/BigLabel.h"
#include "Common/Basic/DarkMagicMath.h"


BEGIN_NS_TRACEFIELD

TraceString TraceModelFactory::stringWithString(const string& Str) const {
    TraceString String;

    for (auto I = Str.begin(), E = Str.end(); I != E; ++I) {
        string C{*I};
        TraceGlyph G = glyphWithString(C);
        String.appendGlyph(std::move(G));
    }
    
    String.refreshBoundingBox();
    return String;
}

TraceGlyph TraceModelFactory::glyphWithString(const string& Str) const
{
    // NB(xenosoz, 2016): Data from archive models are centered at origin (0, 0),
    //   and has no size info. What we should do here is construct a rectangle
    //   which has bottom left corner at origin (0, 0) and top right corner at (w, h).
    //
    //   To fill the gap for the missing size info, we render an invisible font
    //   and extract size information there.
    
    TraceFieldDepot Depot;
    TraceKnotList KnotList = Depot.knotListForGlyphArchive(Str);

    Rect RoughBB = ([&] {
        const float FontSize = 500.f;
        
        // XXX: dynamic font name?
        BigLabel* L = BigLabel::createWithTTF(Str, Depot.defaultFont(), FontSize);
        Size CS = L->getContentSize();
        Rect It = Rect(0.f, 0.f, CS.width, CS.height);
        
        return It;
    }());
    
    return glyphWithKnotList(KnotList, RoughBB);
}

TraceGlyph TraceModelFactory::glyphWithKnotList(const TraceKnotList& KnotList,
                                                Rect RoughBB) const
{
    auto& KnotPoints = KnotList.knotPoints();
    Vec2 PostTranslate(RoughBB.size.width/2.f, RoughBB.size.height/2.f);
    

    //
    // NB(xenosoz): Make the (non-uniform) curve be uniform.
    //   It's not a constant "Speed" of curve for each given time T.
    //   So we define "Energy" here,
    //   Code for update glyph appears 2 times here. Be careful.
    //
    TraceGlyph Glyph;
    DarkMagicMath Math;

    const size_t Count = KnotPoints.size();
    const float EnergyForUnitStep = 1.5f;
    float EnergyLeft = 0.f;
    float LastTimeInSpline = 0.f;
    TraceStroke Stroke;

    size_t C = 0;
    while (C + 3 < Count) {
        TraceKnotPoint KPA = KnotPoints[C + 0];
        TraceKnotPoint KPB = KnotPoints[C + 1];
        TraceKnotPoint KPC = KnotPoints[C + 2];
        TraceKnotPoint KPD = KnotPoints[C + 3];

        // NB(xenosoz): We need 4 points to make a spline.
        bool IsGood = true;
        IsGood = IsGood && (KPA.type() == TraceKnotPoint::TraceKnotPointType::Point);
        IsGood = IsGood && (KPB.type() == TraceKnotPoint::TraceKnotPointType::Point);
        IsGood = IsGood && (KPC.type() == TraceKnotPoint::TraceKnotPointType::Point);
        IsGood = IsGood && (KPD.type() == TraceKnotPoint::TraceKnotPointType::Point);
        if (!IsGood) {
            // Update glyph #1.
            if (!Stroke.samplePoints().empty()) {
                Glyph.appendStroke(std::move(Stroke));
                Stroke = TraceStroke();
            }
            EnergyLeft = 0.f;
            LastTimeInSpline = 0.f;
            C += 1;
            continue;
        }

        Point PA = KPA.point();
        Point PB = KPB.point();
        Point PC = KPC.point();
        Point PD = KPD.point();

        Vec2 Velocity = Math.CatmullRomTangential(PA, PB, PC, PD, LastTimeInSpline);
        float Speed = Math.DistanceFromOrigin(Velocity);

        float AdvanceTime = EnergyLeft / Speed;
        float NewTimeInSpline = LastTimeInSpline + AdvanceTime;

        if (NewTimeInSpline > 1.f) {
            // NB(xenosoz, 2016): Time to advance to next spline.
            //   We're using PC, for the spline segment is [PB, PC].
            Point LastPoint = Math.CatmullRom(PA, PB, PC, PD, LastTimeInSpline);
            float EnergyUsed = Math.DistanceBetweenPoints(PC, LastPoint);

            EnergyLeft = max(0.f, EnergyLeft - EnergyUsed);
            LastTimeInSpline = 0.f;
            C += 1;
            continue;
        }

        Point NewPoint = Math.CatmullRom(PA, PB, PC, PD, NewTimeInSpline);
        Vec2 NewTangent = Math.CatmullRomTangential(PA, PB, PC, PD, NewTimeInSpline);
        NewPoint += PostTranslate;

        TracePoint NewTracePoint(NewPoint, NewTangent);
        Stroke.appendPoint(std::move(NewTracePoint));

        // NB(xenosoz, 2016): You're filled with DETERMINATION here.
        EnergyLeft = EnergyForUnitStep;
        LastTimeInSpline = NewTimeInSpline;
    }

    // Update glyph #2.
    if (!Stroke.samplePoints().empty()) {
        Glyph.appendStroke(std::move(Stroke));
        Stroke = TraceStroke();
    }
    
    Glyph.refreshBoundingBox(RoughBB);
    return Glyph;
}

TraceString TraceModelFactory::resizeStringVertically(const TraceString& Source,
                                                      AARect TargetRect,
                                                      bool UseTightBB /* = false */) const
{
    // NB(xenosoz, 2016): BoundingBox height -> TargetHeight. Preserving the center point.
    //   I just couldn't find a handy Mat3 to utilize.

    AARect BB = (UseTightBB ? Source.tightBoundingBox() : Source.roughBoundingBox());
    Vec2 PreTranslate = -Vec2(BB.minX(), BB.minY());
    float Scale = TargetRect.size.height / BB.size.height;
    Vec2 PostTranslate = Vec2(TargetRect.minX(), TargetRect.minY());

    TraceString S = transformedString(Source, PreTranslate, Scale, PostTranslate);
    return S;
}

TraceString TraceModelFactory::resizeString(const TraceString& Source,
                                            AARect TargetRect,
                                            bool UseTightBB /* = false */) const
{
    // NB(xenosoz, 2016): BoundingBox -> TargetRect. Center to center.
    //   I just couldn't find a handy Mat3 to utilize.

    AARect BB = (UseTightBB ? Source.tightBoundingBox() : Source.roughBoundingBox());
    Vec2 PreTranslate = -Vec2(BB.minX(), BB.minY());
    float Scale = min(TargetRect.size.width / BB.size.width,
                      TargetRect.size.height / BB.size.height);
    Vec2 PostTranslate = Vec2(TargetRect.minX(), TargetRect.minY());
    
    TraceString S = transformedString(Source, PreTranslate, Scale, PostTranslate);
    return S;
}

TraceString
TraceModelFactory::transformedString(const TraceString& SourceString,
                                     Vec2 PreTranslate,
                                     float Scale,
                                     Vec2 PostTranslate) const
{
    TraceString NewString;

    for (auto& Glyph : SourceString)
    {
        TraceGlyph NewGlyph;
        for (auto& Stroke : Glyph.strokes())
        {
            TraceStroke NewStroke;
            for (auto& SamplePoint : Stroke.samplePoints())
            {
                Point P = (SamplePoint.Position + PreTranslate) * Scale + PostTranslate;
                Vec2 V = (SamplePoint.Velocity * Scale);
                NewStroke.appendPoint(TracePoint(P, V));
            }
            NewGlyph.appendStroke(NewStroke);
        }

        Rect RoughBB = Glyph.roughBoundingBox();
        RoughBB.origin = (RoughBB.origin + PreTranslate) * Scale + PostTranslate;
        RoughBB.size = (RoughBB.size * Scale);

        NewGlyph.refreshBoundingBox(RoughBB);
        NewString.appendGlyph(NewGlyph);
    }

    NewString.refreshBoundingBox();
    return NewString;
}

END_NS_TRACEFIELD
