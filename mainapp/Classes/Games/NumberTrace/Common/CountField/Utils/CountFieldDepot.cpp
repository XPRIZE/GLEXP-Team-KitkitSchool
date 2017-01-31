//
//  CountFieldDepot.cpp
//  TodoSchool on Jun 21, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CountFieldDepot.h"

// NB(xenosoz, 2016): Air nodes.
#include "../ChildNodes/Bee.h"
#include "../ChildNodes/BlueButterfly.h"
#include "../ChildNodes/Moth.h"
#include "../ChildNodes/YellowButterfly.h"

// NB(xenosoz, 2016): Ground nodes.
#include "../ChildNodes/Ant.h"
#include "../ChildNodes/Beetle.h"
#include "../ChildNodes/Cockroach.h"
#include "../ChildNodes/LadyBug.h"
#include "../ChildNodes/Spider.h"
#include "../ChildNodes/StagBeetle.h"

#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>


BEGIN_NS_COUNTFIELD

string CountFieldDepot::assetPrefix() const {
    return "NumberTrace";
}

Size CountFieldDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size CountFieldDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Sprite* CountFieldDepot::createBackgroundSprite() {
    Sprite* It = Sprite::create(assetPrefix() + "/Trace_Background.png");
    It->setAnchorPoint(Vec2(0.f, 0.f));
    It->setPosition(Point(0.f, 0.f));
    //It->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    It->setBlendFunc(BlendFunc::DISABLE);

    return It;
}
    
CountObject* CountFieldDepot::createCountObjectWithType(const string& TypeString) {
    // NB(xenosoz, 2016): Air nodes.
    if (TypeString == "Bee") { return Bee::create(); }
    if (TypeString == "BlueButterfly") { return BlueButterfly::create(); }
    if (TypeString == "Moth") { return Moth::create(); }
    if (TypeString == "YellowButterfly") { return YellowButterfly::create(); }
    
    // NB(xenosoz, 2016): Ground nodes.
    if (TypeString == "Ant") { return Ant::create(); }
    if (TypeString == "Beetle") { return Beetle::create(); }
    if (TypeString == "Cockroach") { return Cockroach::create(); }
    if (TypeString == "Ladybug") { return Ladybug::create(); }
    if (TypeString == "Spider") { return Spider::create(); }
    if (TypeString == "StagBeetle") { return StagBeetle::create(); }

    CCLOGERROR("Unknown typestring in CountFieldDepot::createCountObjectWithType: '%s'", TypeString.c_str());
    return Ant::create();
}

Size CountFieldDepot::sizeForCountObjectWithType(const string& TypeString) {
    // NB(xenosoz, 2016): Air nodes.
    if (TypeString == "Bee") { return Bee::defaultSize(); }
    if (TypeString == "BlueButterfly") { return BlueButterfly::defaultSize(); }
    if (TypeString == "Moth") { return Moth::defaultSize(); }
    if (TypeString == "YellowButterfly") { return YellowButterfly::defaultSize(); }

    // NB(xenosoz, 2016): Ground nodes.
    if (TypeString == "Ant") { return Ant::defaultSize(); }
    if (TypeString == "Beetle") { return Beetle::defaultSize(); }
    if (TypeString == "Cockroach") { return Cockroach::defaultSize(); }
    if (TypeString == "Ladybug") { return Ladybug::defaultSize(); }
    if (TypeString == "Spider") { return Spider::defaultSize(); }
    if (TypeString == "StagBeetle") { return StagBeetle::defaultSize(); }
    
    CCLOGERROR("Unknown typestring in CountFieldDepot::createCountObjectWithType: '%s'", TypeString.c_str());
    return Ant::defaultSize();
}
    
Poses CountFieldDepot::makePosesWithMultipleTrial(size_t N, Size SpriteSize,
                                                  Point PositionOffset, Size Boundary,
                                                  float RotationBegin, float RotationEnd) const
{
    int TrialCount = 0;
    const int TrialLimit = 20;
    
    Poses PosesResult;
    
    while (PosesResult.size() < N && TrialCount++ < TrialLimit) {
        PosesResult = makePoses(N, SpriteSize,
                                PositionOffset, Boundary,
                                RotationBegin, RotationEnd);
    }
    return PosesResult;
}

Poses CountFieldDepot::makePoses(size_t N, Size SpriteSize,
                                 Point PositionOffset, Size Boundary,
                                 float RotationBegin, float RotationEnd) const
{
    Poses PosesResult;
    
    int TrialCount = 0;
    const int TrialLimit = 20;
    float SpriteDiagonal = Vec2(SpriteSize.width, SpriteSize.height).length();
    
    // NB(xenosoz, 2016): Inset.
    Boundary.width -= SpriteDiagonal;
    Boundary.height -= SpriteDiagonal;
    
    while (PosesResult.size() < N && TrialCount++ < TrialLimit) {
        float RandomX = random(-Boundary.width/2.f, Boundary.width/2.f);
        float RandomY = random(-Boundary.height/2.f, Boundary.height/2.f);

        Pose NewPose;
        NewPose.Position = PositionOffset + Point(RandomX, RandomY);
        NewPose.Rotation = random(RotationBegin, RotationEnd);

        bool CollisionFound = false;
        for (auto& Pose : PosesResult) {
            float D = NewPose.Position.distance(Pose.Position);
            if (D <= SpriteDiagonal) {
                CollisionFound = true;
                break;
            }
        }
        
        if (CollisionFound) { continue; }
        
        PosesResult.push_back(NewPose);
    }
    
    if (PosesResult.size() < N) {
        // NB(xenosoz, 2016): Return empty poses for error.
        PosesResult.clear();
    }
    
    return PosesResult;
}

float CountFieldDepot::frameRate() const {
    return 30.f;
}

Sprite* CountFieldDepot::createSpriteForSpriteFrames(const SpriteFrames& Frames) const {
    if (Frames.empty()) {
        CCLOGERROR("Got empty frames %p in CountFieldDepot::createSpriteForSpriteFrames)", &Frames);
        return nullptr;
    }
    
    Sprite* It = Sprite::createWithSpriteFrame(Frames.front());
    It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    It->setPosition(Point::ZERO);
    //It->setBlendFunc((BlendFunc){GL_ZERO, GL_ONE_MINUS_SRC_ALPHA});

    return It;
}

Action* CountFieldDepot::createLoopActionForSpriteFrames(const SpriteFrames& Frames) const {
    float FrameRate = frameRate();
    Animation* A = Animation::createWithSpriteFrames(Frames, 1.f / FrameRate);
    
    Animate* AnimateAction = Animate::create(A);
    RepeatForever* RepeatAction = RepeatForever::create(AnimateAction);
    
    return RepeatAction;
}
 
CountFieldDepot::SpriteFrames CountFieldDepot::framesByFileDetails(const string& Format,
                                                                   size_t FrameCount,
                                                                   Size S) const
{    
    Rect R(Point::ZERO, S);

    CountFieldDepot::SpriteFrames Frames;
    for (int FrameID = 1; FrameID <= (int)FrameCount; ++FrameID) {
        string Filename = StringUtils::format(Format.c_str(), FrameID);
        Frames.pushBack(SpriteFrame::create(Filename, R));
    }
    return Frames;
}

END_NS_COUNTFIELD
