//
//  LittleButterfly.h on May 18, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_COUNTFIELD_LITTLEBUTTERFLY_H
#define TODOSCHOOL_COUNTFIELD_LITTLEBUTTERFLY_H

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <Games/NumberTrace/Common/Basic/Cocos2dFwd.h>
#include <Games/NumberTrace/Common/Basic/TodoSchoolNode.h>
#include <cocos/2d/CCNode.h>
#include <cocos/base/CCRefPtr.h>
#include <functional>

namespace todoschool {
namespace countfield {

class LittleButterfly: public TodoSchoolNode<LittleButterfly> {
    typedef cocos2d::Node Super;
    cocos2d::RefPtr<cocos2d::Sprite> TheSprite;
    cocos2d::RefPtr<cocos2d::Action> TheAnimation;

public:
    Chain<bool> Active;

    std::function<void(LittleButterfly*)> OnAnimationDidEnd;

public:
    bool init() override;
    void clear();

private:
    void handleActiveValueUpdated(bool&);
};

} // namespace countfield
} // namespace todoschool

#endif // !defined(TODOSCHOOL_COUNTFIELD_LITTLEBUTTERFLY_H)
