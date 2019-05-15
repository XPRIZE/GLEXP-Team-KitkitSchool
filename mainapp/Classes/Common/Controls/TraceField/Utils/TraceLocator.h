//
//  TraceLocator.h -- Interactions between TraceString and the finger.
//  TodoSchool on Apr 27, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//  which is again from Todomath iOS (Locomotive Labs)
//


#pragma once

#include "../LiveModels/TraceIndex.h"
#include "TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceLocator {
public:
    // NB(xenosoz, 2016): Return TraceIndex::End() if no feasible point found.
    TraceIndex bestIndexByFinger(TraceIndex PassedIndex, const TraceString& String, Point FingerPoint);

    bool isBadFinger(TraceIndex PassedIndex, const TraceString& String, Point FingerPoint);
    bool isItGoodDayToAdvanceStroke(TraceIndex PassedIndex, const TraceString& String);
};

END_NS_TRACEFIELD
