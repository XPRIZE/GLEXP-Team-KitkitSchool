//
//  CocosRepr.h on Apr 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef TODOSCHOOL_REPR_COCOSREPR_H
#define TODOSCHOOL_REPR_COCOSREPR_H

#include "ReprBasic.h"
#include <Games/NumberTrace/Common/Basic/Cocos2dFwd.h>

namespace todoschool {

template<> std::string repr<cocos2d::Size>(const cocos2d::Size& S);
template<> std::string repr<cocos2d::Vec2>(const cocos2d::Vec2& V);
template<> std::string repr<cocos2d::Vec3>(const cocos2d::Vec3& V);
template<> std::string repr<cocos2d::Vec4>(const cocos2d::Vec4& V);
template<> std::string repr<cocos2d::Rect>(const cocos2d::Rect& R);
template<> std::string repr<cocos2d::Touch>(const cocos2d::Touch& T);
template<> std::string repr<cocos2d::Color3B>(const cocos2d::Color3B& C);
template<> std::string repr<cocos2d::Color4B>(const cocos2d::Color4B& C);

}  // namespace todoschool

#endif  // !defined(TODOSCHOOL_REPR_COCOSREPR_H)
