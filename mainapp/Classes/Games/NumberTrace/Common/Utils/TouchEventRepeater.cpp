//
//  TouchEventRepeater.h on May 18, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include <Games/NumberTrace/Common/Utils/TouchEventRepeater.h>
#include <Games/NumberTrace/Common/Utils/DarkMagicMath.h>

#include <cocos/cocos2d.h>
#include <algorithm>
#include <cassert>

using namespace todoschool;
using namespace cocos2d;
using namespace std;

TouchEventRepeater::TouchEventRepeater(FunctionType Fn, int Multiplier)
    : Fn(Fn)
{
    assert(Multiplier >= 0);
    DeltaStep = 1.f / max<float>(1.f, Multiplier);
}

void TouchEventRepeater::operator()(cocos2d::Touch* T, cocos2d::Event* E) const {
    DarkMagicMath Math;

    if (T) {
        // NB(xenosoz, 2016): interpolate fake touch in [StartPoint, StopPoint).
        Vec2 StartPoint = T->getPreviousLocationInView();  // It's not getStartLocationInView().
        Vec2 StopPoint = T->getLocationInView();
        Touch FakeTouch;
        FakeTouch.setTouchInfo(T->getID(), StartPoint.x, StartPoint.y, T->getCurrentForce(), T->getMaxForce());
        for (float Alpha = DeltaStep; Alpha < 1.f; Alpha += DeltaStep) {
            Vec2 Point = Math.MidpointWithAlpha(StartPoint, StopPoint, Alpha);
            FakeTouch.setTouchInfo(T->getID(), Point.x, Point.y, T->getCurrentForce(), T->getMaxForce());
            Fn(&FakeTouch, E);
        }
    }

    Fn(T, E);
}
