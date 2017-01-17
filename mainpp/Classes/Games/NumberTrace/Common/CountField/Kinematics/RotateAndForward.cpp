//
//  RotateAndForward.cpp on Jun 28, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "RotateAndForward.h"
#include <Games/NumberTrace/Common/Utils/DarkMagicMath.h>

using namespace cocos2d;
using namespace std;

namespace todoschool {
namespace countfield {
    
RotateAndForward::RotateAndForward(float Speed, float AngularSpeed)
    : Speed(Speed), AngularSpeed(AngularSpeed)
{
    Scale.update(1.f);
}
    
void RotateAndForward::update(float DeltaSeconds) {
    DarkMagicMath Math;
    
    Point DeltaPosition = TargetPosition() - CurrentPosition();
    
    if (DeltaPosition.length() <= Speed * Scale() * DeltaSeconds) {
        // NB(xenosoz, 2016): We're here in the target position.
        updateToTargetPosition(DeltaSeconds);
        
        float DeltaRotation = Math.NormalizedAngle(TargetRotation() - CurrentRotation(), 0.f);
        if (fabs(DeltaRotation) <= AngularSpeed * DeltaSeconds) {
            // NB(xenosoz, 2016): We're aligned with the target angle.
            updateToTargetAngle(DeltaSeconds);
            return;
        }
        
        // NB(xenosoz, 2016): We need more rotation to the target angle.
        if (DeltaRotation >= 0.f) {
            updateByRotateLeft(DeltaSeconds);
            return;
        }
        
        if (DeltaRotation <= -0.f) {
            updateByRotateRight(DeltaSeconds);
            return;
        }
        
        CCLOGERROR("Hit unreachable code in RotateAndForward::update: %f.", DeltaRotation);
        return;
    }
    
    // NB(xenosoz, 2016): `Rotate to the target position' vs `move forward'.
    float RotationForTargetPosition = atan2(DeltaPosition.y, DeltaPosition.x);
    float DeltaRotation = ([&] {
        float It = RotationForTargetPosition - CurrentRotation();
        It = Math.NormalizedAngle(It, 0.f);
        return It;
    }());
    
    if (fabs(DeltaRotation) <= AngularSpeed * DeltaSeconds) {
        // NB(xenosoz, 2016): We're heading to the target position.
        CurrentRotation.update(RotationForTargetPosition);
        
        updateByMoveStraight(DeltaSeconds);
        return;
    }
    
    // NB(xenosoz, 2016): We need more rotation to the target position.
    if (DeltaRotation >= 0.f) {
        updateByRotateLeft(DeltaSeconds);
        return;
    }
    
    if (DeltaRotation <= -0.f) {
        updateByRotateRight(DeltaSeconds);
        return;
    }
    
    CCLOGERROR("Hit unreachable code in RotateAndForward::update: %f.", DeltaRotation);
}
    
void RotateAndForward::updateToTargetPosition(float) {
    CurrentPosition.update(TargetPosition());
}

void RotateAndForward::updateToTargetAngle(float) {
    CurrentRotation.update(TargetRotation());
}

void RotateAndForward::updateByMoveStraight(float DeltaSeconds) {
    float Angle = CurrentRotation();
    Vec2 Velocity = Speed * Scale() * DeltaSeconds * Vec2(cos(Angle), sin(Angle));
    
    Point P = CurrentPosition() + Velocity;
    CurrentPosition.update(P);
}
    
void RotateAndForward::updateByRotateLeft(float DeltaSeconds) {
    float Angle = CurrentRotation();
    Angle += AngularSpeed * DeltaSeconds;
    
    CurrentRotation.update(Angle);
}
    
void RotateAndForward::updateByRotateRight(float DeltaSeconds) {
    float Angle = CurrentRotation();
    Angle -= AngularSpeed * DeltaSeconds;
    
    CurrentRotation.update(Angle);
}

}  // namespace countfield
}  // namespace todoschool
