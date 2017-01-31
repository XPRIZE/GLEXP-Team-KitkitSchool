//
//  CircleAndStraight.h on Jun 28, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef ENUMAXPRIZE_COUNTFIELD_CIRCLEANDSTRAIGHT_H
#define ENUMAXPRIZE_COUNTFIELD_CIRCLEANDSTRAIGHT_H

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <Games/NumberTrace/Common/ADT/Optional.h>
#include <cocos/cocos2d.h>
#include <array>

namespace todoschool {
namespace countfield {

// NB(xenosoz, 2016): Unicycle kinematic with constant speed and rotational radius.
//   1. Move along the first arc.
//   2. Move along the second arc (or go straight).
//   3. Move along the last arc.

// NB(xenosoz, 2016): Keyword: Dubin's car

class CircleAndStraight {
    // NB(xenosoz, 2016): Speed means the natural speed (at scale=1.0).
    //   We need to scale it properly.
    float Speed;

    // NB(xenosoz, 2016): But we don't need to scale the angular speed.
    float AngularSpeed;

    float EngineUptime;

public:
    Chain<cocos2d::Point> CurrentPosition;
    Chain<cocos2d::Point> TargetPosition;
    
    // NB(xenosoz, 2016): Rotations are in radians.
    Chain<float> CurrentRotation;
    Chain<float> TargetRotation;
    
    // NB(xenosoz, 2016): Time limit to the movement.
    Optional<float> EngineTimeLimit;

    Chain<float> Scale;

    std::function<void(CircleAndStraight*)> OnMovingDidEnd;

public:
    CircleAndStraight(float Speed, float Radius);
    void clear();
    void update(float DeltaSeconds);

private:
    void updateToTargetPosition(float DeltaSeconds);
    void updateToTargetAngle(float DeltaSeconds);
    void updateByMoveStraight(float DeltaSeconds);
    void updateByTurnLeft(float DeltaSeconds);
    void updateByTurnRight(float DeltaSeconds);

    struct Command {
        float Left;
        float Straight;
        float Right;
        
        Command();
        float sum();
    };
    struct Solution {
        bool Valid;
        std::array<Command, 3> Commands;
        
        Solution();
        float distance();
    };
    
    // NB(xenosoz, 2016): Consult
    //   "Classification of the Dubins set by Andrei M. Shkel, Vladimir Lumelsky".
    float offsetRotation();
    float alpha();
    float beta();
    
    float radius();
    float distance();
    
    Solution solveLSL();  // Case #1
    Solution solveRSR();  // Case #2
    Solution solveLSR();  // Case #3
    Solution solveRSL();  // Case #4
    Solution solveRLR();  // Case #5
    Solution solveLRL();  // Case #6
    Solution solve();
};
    
}  // namespace countfield
}  // namespace todoschool

#endif  // !defined(ENUMAXPRIZE_COUNTFIELD_CIRCLEANDSTRAIGHT_H)
