//
//  CountFieldDepot.h
//  TodoSchool on Jun 21, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#pragma once

#include "../Models/Pose.h"
#include "CountFieldNS.h"


BEGIN_NS_COUNTFIELD

class CountObject;

class CountFieldDepot {
public:
    typedef Vector<SpriteFrame*> SpriteFrames;
    
    string assetPrefix() const;
    Size windowSize() const;
    Size gameSize() const;

    Sprite* createBackgroundSprite();
    CountObject* createCountObjectWithType(const string& TypeString);
    Size sizeForCountObjectWithType(const string& TypeString);
    
    // NB(xenosoz, 2016): Return empty poses on error.
    Poses makePosesWithMultipleTrial(size_t N, Size SpriteSize,
                                     Point PositionOffset, Size Boundary,
                                     float RotationBegin, float RotationEnd) const;
    Poses makePoses(size_t N, Size SpriteSize,
                    Point PositionOffset, Size Boundary,
                    float RotationBegin, float RotationEnd) const;
    
    float frameRate() const;
    Sprite* createSpriteForSpriteFrames(const SpriteFrames& Frames) const;
    Action* createLoopActionForSpriteFrames(const SpriteFrames& Frames) const;

    SpriteFrames framesByFileDetails(const string& Format,
                                     size_t FrameCount,
                                     Size S) const;
};

END_NS_COUNTFIELD
