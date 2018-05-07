//
//  Pose.h on Jun 28, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#ifndef ENUMAXPRIZE_COUNTFIELD_POSE_H
#define ENUMAXPRIZE_COUNTFIELD_POSE_H

#include <cocos/cocos2d.h>
#include <vector>


namespace todoschool {
namespace countfield {

class Pose {
public:
    cocos2d::Point Position;

    // NB(xenosoz, 2016): Rotation is in radians.
    float Rotation;
    
public:
    Pose() = default;
    Pose(const cocos2d::Point& Position, float Rotation);
};
    
using Poses = std::vector<Pose>;
    
}  // namespace countfield
}  // namespace todoschool

#endif  // !defined(ENUMAXPRIZE_COUNTFIELD_POSE_H)
