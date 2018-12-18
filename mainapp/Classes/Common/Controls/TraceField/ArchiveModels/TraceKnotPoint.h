//
//  TraceKnotPoint.h -- A control point of trace fonts
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//

//
// NB(xenosoz, 2016): TraceKnotPoint is basically a control point of catmull-rom spline (type Point).
//
//   A path segment b-c is made of 4 consecutive points (a, b, c, d).
//   A separator (type Separator) *separates* those path segments. i.e.
//     >>> KnotPoints(a, s, d, f, g, SEP, q, w, e, r)
//     three path segments s-d, d-f, and w-e.
//
//   A stop (type Stop) *ends* the sequence, finishing a letter, Word, etc. i.e.
//     >>> KnotPoints(a, s, d, f, SEP, q, w, e, r, L_STOP, z, x, c, v)
//     Two letters. Letter(s-d, w-e) and Letter(x-c).
//
//   Consecutive point group which length is less than 4 is just ignored. i.e.
//     >>> KnotPoints(a, s, d, SEP, q, SEP, z, x)
//     It doesn't gives path segments at all.
//


#pragma once

#include "../Utils/TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceKnotPoint {
public:
    enum class TraceKnotPointType {
        Point,
        Separator,
        LetterStop,
        WordStop
    };

private:
    float X;
    float Y;
    TraceKnotPointType Type;

public:
    explicit TraceKnotPoint(float X, float Y, TraceKnotPointType Type);
    explicit TraceKnotPoint(Json::Value& P);

    static TraceKnotPointType createTypeWithString(const string& TypeString);

    float x() const;
    float y() const;
    cocos2d::Point point() const;
    TraceKnotPointType type() const;
};

END_NS_TRACEFIELD
