//
//  WalkOut.cpp on Jul 13, 2016.
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WalkOut.h"
#include <memory>

using namespace cocos2d;
using namespace std;


namespace todoschool {
namespace countfield {

WalkOut* WalkOut::create(float Duration,
                         const Pose& StartPose,
                         const Pose& StopPose,
                         float Scale,
                         float Speed,
                         float AngularSpeed)
{
    std::unique_ptr<WalkOut> Ret(new(std::nothrow) WalkOut(Speed, AngularSpeed));
    if (Ret && Ret->init(Duration, StartPose, StopPose, Scale, Speed, AngularSpeed))
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    return Ret.release();
}

WalkOut::WalkOut(float Speed, float AngularSpeed)
    : Engine(Speed, AngularSpeed) {
}

bool WalkOut::init(float Duration,
                   const Pose& StartPose,
                   const Pose& StopPose,
                   float Scale,
                   float Speed,
                   float AngularSpeed)
{
    if (!Super::initWithDuration(Duration))
        return false;
    
    this->Duration = Duration;
    this->StartPose = StartPose;
    this->StopPose = StopPose;
    this->Scale = Scale;
    this->Speed = Speed;
    this->AngularSpeed = AngularSpeed;
    
    return true;
}

WalkOut* WalkOut::clone() const {
    std::unique_ptr<WalkOut> Ret(new(std::nothrow) WalkOut(Speed, AngularSpeed));
    if (Ret && Ret->init(Duration, StartPose, StopPose, Scale, Speed, AngularSpeed))
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    return Ret.release();
}

void WalkOut::startWithTarget(Node* Target) {
    Super::startWithTarget(Target);
}

void WalkOut::update(float Time) {
    Super::update(Time);
    
    // NB(xenosoz, 2016): Invert causality and effect.
    //   (I just couldn't get quite amount of numerical stability by other means.)
    Engine.clear();
    Engine.CurrentPosition.update(StartPose.Position);
    Engine.CurrentRotation.update(StartPose.Rotation);
    Engine.TargetPosition.update(StopPose.Position);
    Engine.TargetRotation.update(StopPose.Rotation);
    Engine.Scale.update(Scale);

    // XXX: One call of Engine.update should be suffice. But it's not now :(
    const int SplitCount = 25;
    float CurrentTime = Time * Duration;
    float TimeSlice = CurrentTime / SplitCount;
    
    for (int SplitID = 0; SplitID < SplitCount; ++SplitID) {
        Engine.update(TimeSlice);
    }
    
    Point P = Engine.CurrentPosition();
    float R = 90.f - MATH_RAD_TO_DEG(Engine.CurrentRotation());
    getTarget()->setPosition(P);
    getTarget()->setRotation(R);
}


}  // namespace countfield
}  // namespace todoschool

