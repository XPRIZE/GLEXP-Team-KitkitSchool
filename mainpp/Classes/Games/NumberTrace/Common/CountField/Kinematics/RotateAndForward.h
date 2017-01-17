//
//  RotateAndForward.h on Jun 28, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef ENUMAXPRIZE_COUNTFIELD_ROTATEANDFORWARD_H
#define ENUMAXPRIZE_COUNTFIELD_ROTATEANDFORWARD_H

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <cocos/cocos2d.h>

namespace todoschool {
namespace countfield {

// NB(xenosoz, 2016): Unicycle kinematic with constant speed and rotational speed.
//   1. Rotate to the target position.
//   2. Move forward.
//   3. Align with the target angle.
    
class RotateAndForward {
    // NB(xenosoz, 2016): Speed means the natural speed (at scale=1.0).
    //   We need to scale it properly.
    float Speed;

    // NB(xenosoz, 2016): But we don't need to scale the angular speed.
    float AngularSpeed;
    
public:
    Chain<cocos2d::Point> CurrentPosition;
    Chain<cocos2d::Point> TargetPosition;
    
    // NB(xenosoz, 2016): Rotations are in radians.
    Chain<float> CurrentRotation;
    Chain<float> TargetRotation;
    
    Chain<float> Scale;
    
public:
    RotateAndForward(float Speed, float AngularSpeed);
    void update(float DeltaSecond);
    
private:
    void updateToTargetPosition(float DeltaSeconds);
    void updateToTargetAngle(float DeltaSeconds);
    void updateByMoveStraight(float DeltaSeconds);
    void updateByRotateLeft(float DeltaSeconds);
    void updateByRotateRight(float DeltaSeconds);
};

}  // namespace countfield
}  // namespace todoschool

#endif  // !defined(ENUMAXPRIZE_COUNTFIELD_ROTATEANDFORWARD_H)
