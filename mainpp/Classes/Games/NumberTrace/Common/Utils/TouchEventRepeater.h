//
//  TouchEventRepeater.h on May 18, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_UTILS_TOUCHEVENTREPEATER_H
#define TODOSCHOOL_UTILS_TOUCHEVENTREPEATER_H

#include <Games/NumberTrace/Common/Basic/Cocos2dFwd.h>
#include <functional>
namespace todoschool {

class TouchEventRepeater {
public:
    typedef std::function<void(cocos2d::Touch*, cocos2d::Event*)> FunctionType;

private:
    float DeltaStep;
    FunctionType Fn;

public:
    TouchEventRepeater(FunctionType Fn, int Multiplier);
    void operator()(cocos2d::Touch* T, cocos2d::Event* E) const;
};

} // namespace todoschool
#endif //!defined(TODOSCHOOL_UTILS_TOUCHEVENTREPEATER_H)
