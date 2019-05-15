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

#include "./TraceLocator.h"

#include "../LiveModels/TraceStroke.h"
#include "../LiveModels/TraceGlyph.h"
#include "../LiveModels/TraceString.h"
#include "../LiveModels/TraceIndex.h"


BEGIN_NS_TRACEFIELD

namespace {
    // for allowing drawing error for any direction
    const float distanceLimit = 160.f;  // formerly known as guestimateThreshold.
    
    // NB(xenosoz, 2016): energy* are for setting lookahead buffers
    //   energyLimit(150.f) seems too stiff for 'latin small letter a'.
    const float energyLimit = 500.f;
    const float energyPerDistance = .8f;
    const float energyPerDegree = .5f;
    const float energyMaxDegree = 20.f;
    
    // When we move finger too far from the stroke, it shows us animation.
    // We call it BadFinger if finger is too far from the stroke.
    const float badFingerDistance = 10.f;
    //const float badFingerAnimationDuration = 0.8f;  // XXX: unused
    
    // Ignore last part and advance to the next stroke if possible.
    const float strokeSkipDistanceLimit = 700.f;  // formerly known as lastAdvanceThreshold
}  // unnamed namespace


TraceIndex TraceLocator::bestIndexByFinger(TraceIndex PassedIndex, const TraceString& String, Point FingerPoint)
{
    TraceIndex BestIndex = TraceIndex::end();
    float BestDistance = distanceLimit;
    float Energy = energyLimit;

    // NB(xenosoz, 2016): Limiting range from passed index to this stroke.
    TraceIndex Start = PassedIndex;
    TraceIndex Stop = PassedIndex.indexForStrokeEnd(String);

    if (Start < Stop) {
        TracePoint LastItem = Start.pointFor(String);

        for (TraceIndex I = Start, E = Stop; I != E; I.increment(String)) {
            TracePoint Item = I.pointFor(String);
            float DistanceToLast = Item.Position.distance(LastItem.Position);
            float DistanceToFinger = Item.Position.distance(FingerPoint);

            float ThetaInRadian = (float)atan2(LastItem.Velocity.y, LastItem.Velocity.x);
            float ThetaInDegree = ThetaInRadian * 180.f / (float)M_PI;

            Energy -= DistanceToLast * energyPerDistance;
            Energy -= min(fabsf(ThetaInDegree), energyMaxDegree) * energyPerDegree;
            if (Energy < 0.f) {
                // NB(xenosoz, 2016): No energy left. Stop.
                break;
            }
            if (DistanceToFinger < BestDistance) {
                BestDistance = DistanceToFinger;
                BestIndex = I;
            }
            LastItem = Item;
        }
    }

    return BestIndex;
}

bool TraceLocator::isBadFinger(TraceIndex PassedIndex, const TraceString& String, Point FingerPoint) {
    TracePoint Item = PassedIndex.pointFor(String);
    float DistanceToFinger = Item.Position.distance(FingerPoint);

    return DistanceToFinger >= badFingerDistance;
}

bool TraceLocator::isItGoodDayToAdvanceStroke(TraceIndex PassedIndex, const TraceString& String) {
    TraceIndex Start = PassedIndex;
    TraceIndex Stop = PassedIndex.indexForStrokeEnd(String);

    float TailDistance = 0.f;
    TracePoint LastItem = PassedIndex.pointFor(String);
    for (TraceIndex I = Start, E = Stop; I != E; I.increment(String)) {
        TracePoint Item = I.pointFor(String);
        float Distance = LastItem.Position.distance(Item.Position);

        TailDistance += Distance;
        if (TailDistance > strokeSkipDistanceLimit) {
            return false;
        }
    }
    return true;
}

}  // namespace tracefield
}  // namespace todoschool
