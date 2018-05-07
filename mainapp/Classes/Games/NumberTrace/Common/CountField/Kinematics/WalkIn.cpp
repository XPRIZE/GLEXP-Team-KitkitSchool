//
//  WalkIn.cpp on Jul 12, 2016.
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WalkIn.h"
#include <memory>

using namespace cocos2d;
using namespace std;


namespace todoschool {
namespace countfield {
        
WalkIn* WalkIn::create(float Duration,
                       const Pose& StartPose,
                       const Pose& StopPose,
                       float Scale,
                       float Speed,
                       float AngularSpeed)
{
    std::unique_ptr<WalkIn> Ret(new(std::nothrow) WalkIn(Speed, AngularSpeed));
    if (Ret && Ret->init(Duration, StartPose, StopPose, Scale, Speed, AngularSpeed))
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    return Ret.release();
}

WalkIn::WalkIn(float Speed, float AngularSpeed)
    : Engine(Speed, AngularSpeed) {
}
    
bool WalkIn::init(float Duration,
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

WalkIn* WalkIn::clone() const {
    std::unique_ptr<WalkIn> Ret(new(std::nothrow) WalkIn(Speed, AngularSpeed));
    if (Ret && Ret->init(Duration, StartPose, StopPose, Scale, Speed, AngularSpeed))
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    return Ret.release();
}

void WalkIn::startWithTarget(Node* Target) {
    Super::startWithTarget(Target);
}

void WalkIn::update(float Time) {
    // NB(xenosoz, 2017): Calling Super::update here just says "[Action update]. override me" in every frame which is very annoying.
    //   Super::update(Time);
    
    // NB(xenosoz, 2016): Invert causality and effect.
    //   (I just couldn't get quite amount of numerical stability by other means.)
    Engine.clear();
    Engine.CurrentPosition.update(StopPose.Position);
    Engine.CurrentRotation.update(StopPose.Rotation - M_PI);
    Engine.TargetPosition.update(StartPose.Position);
    Engine.TargetRotation.update(StartPose.Rotation - M_PI);
    Engine.Scale.update(Scale);

    // XXX: One call of Engine.update should be suffice. But it's not now :(
    const int SplitCount = 25;
    float CurrentTime = (1.f - Time) * Duration;
    float TimeSlice = CurrentTime / SplitCount;

    for (int SplitID = 0; SplitID < SplitCount; ++SplitID) {
        Engine.update(TimeSlice);
    }

    Point P = Engine.CurrentPosition();
    float R = (90.f - MATH_RAD_TO_DEG(Engine.CurrentRotation())) + 180.f;
    getTarget()->setPosition(P);
    getTarget()->setRotation(R);
}


}  // namespace countfield
}  // namespace todoschool

