//
//  TraceKnotList.h -- A sequence of TraceKnotPoint
//  TodoSchool on Apr 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//  which is again from Todomath iOS (Locomotive Labs)
//

#include "TraceKnotList.h"


BEGIN_NS_TRACEFIELD

TraceKnotList::TraceKnotList() {}

TraceKnotList::TraceKnotList(Json::Value& Points) {
    extend(Points);
}

void TraceKnotList::appendKnotPoint(const TraceKnotPoint& KnotPoint) {
    KnotPoints.push_back(KnotPoint);
}

void TraceKnotList::appendKnotPoint(TraceKnotPoint&& KnotPoint) {
    KnotPoints.push_back(std::move(KnotPoint));
}

void TraceKnotList::clear() {
    KnotPoints.clear();
}

void TraceKnotList::extend(const TraceKnotList& KnotList) {
    KnotPoints.insert(KnotPoints.end(), KnotList.KnotPoints.begin(), KnotList.KnotPoints.end());
}

const vector<TraceKnotPoint>& TraceKnotList::knotPoints() const {
    return KnotPoints;
}

void TraceKnotList::extend(Json::Value& Points) {
    for (int i = 0; i < Points.size(); ++i) {
        TraceKnotPoint KnotPoint(Points[i]);
        KnotPoints.push_back(std::move(KnotPoint));
    }
}

END_NS_TRACEFIELD