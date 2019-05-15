//
//  TraceKnotList.h -- A sequence of TraceKnotPoint
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//

//
// NB(xenosoz, 2016): In fact, it's really not a linked list.
//   But I just wanted to avoid name like TraceKnotPoints.
//

#pragma once

#include "TraceKnotPoint.h"
#include "../Utils/TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceKnotList {
    vector<TraceKnotPoint> KnotPoints;

public:
    TraceKnotList();
    TraceKnotList(Json::Value& Points);

    void appendKnotPoint(const TraceKnotPoint& KnotPoint);
    void appendKnotPoint(TraceKnotPoint&& KnotPoint);
    void clear();
    void extend(const TraceKnotList& KnotList);
    void extend(Json::Value& Points);
    const vector<TraceKnotPoint>& knotPoints() const;
};

END_NS_TRACEFIELD
