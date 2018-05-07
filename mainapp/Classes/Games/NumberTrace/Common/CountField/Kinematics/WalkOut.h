//
//  WalkOut.h on Jul 13, 2016.
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#ifndef TODOSCHOOL_COUNTFIELD_WALKOUT_H
#define TODOSCHOOL_COUNTFIELD_WALKOUT_H

#include "CircleAndStraight.h"
#include "../Models/Pose.h"
#include <cocos/cocos2d.h>


namespace todoschool {
namespace countfield {

class WalkOut : public cocos2d::ActionInterval {
    typedef cocos2d::ActionInterval Super;
    typedef CircleAndStraight EngineType;
    
    float Duration;
    Pose StartPose;
    Pose StopPose;
    float Scale;
    float Speed;
    float AngularSpeed;
    
    EngineType Engine;
    
public:
    static WalkOut* create(float Duration,
                           const Pose& StartPose,
                           const Pose& StopPose,
                           float Scale,
                           float Speed,
                           float AngularSpeed);
    WalkOut(float Speed, float AngularSpeed);
    
    bool init(float Duration,
              const Pose& StartPose,
              const Pose& StopPose,
              float Scale,
              float Speed,
              float AngularSpeed);
    
    WalkOut* clone() const override;
    void startWithTarget(cocos2d::Node* Target) override;
    
    // NB(xenosoz, 2016): Time is in [0, 1]. We need to scale it by Duration.
    void update(float Time) override;
};

}  // namespace countfield
}  // namespace todoschool

#endif  // !defined(TODOSCHOOL_COUNTFIELD_WALKOUT_H)
