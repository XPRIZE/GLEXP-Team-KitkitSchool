//
//  LMCardSlotBuilder.h on Aug 8, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <cocos/cocos2d.h>
#include <vector>


class LMCardSlotBuilder {
public:
    std::vector<cocos2d::Point> pointsInBoundary(size_t cardCount,
                                                 cocos2d::Rect boardRect,
                                                 cocos2d::Size cardSize);
    
    std::vector<cocos2d::Point> candidatePointsInBoundary(size_t cardCount,
                                                          cocos2d::Rect boardRect,
                                                          cocos2d::Size cardSize);
};
