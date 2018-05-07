//
//  StarFallNS.h on Aug 2, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#define BEGIN_NS_STARFALL \
namespace todoschool { \
namespace starfall {

#define END_NS_STARFALL \
} \
}

#include <Common/ADT/Chain.h>
#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/NodeScopeGuard.h"
#include "Common/Repr/AllRepr.h"
#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoSchoolBackButton.hpp>
#include <Common/Controls/ProgressIndicator.hpp>
#include <Common/Controls/CompletePopup.hpp>

#include <Utils/TodoUtil.h>

#include <cocos/cocos2d.h>
#include <cocos/ui/UIButton.h>
#include <cocos/ui/UIWidget.h>
#include <3rdParty/json/json.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>


BEGIN_NS_STARFALL

inline std::string itos(int I) { return TodoUtil::itos(I); }
inline int stoi(const std::string& S) { return TodoUtil::stoi(S); }

using cocos2d::Vec2;
using cocos2d::Point;
using cocos2d::Size;
using cocos2d::Rect;
using cocos2d::Color3B;
using cocos2d::Color4B;
using cocos2d::BlendFunc;

using cocos2d::Node;
using cocos2d::Label;
using cocos2d::Sprite;
using cocos2d::ui::Button;
using cocos2d::ui::Widget;

using cocos2d::FiniteTimeAction;
using cocos2d::Sequence;
using cocos2d::Spawn;
using cocos2d::DelayTime;
using cocos2d::CallFunc;
using cocos2d::MoveBy;
using cocos2d::MoveTo;
using cocos2d::BezierBy;
using cocos2d::BezierTo;
using cocos2d::EaseIn;
using cocos2d::EaseOut;
using cocos2d::FadeIn;
using cocos2d::FadeOut;
using cocos2d::FadeTo;

using cocos2d::random;
using cocos2d::StringUtils::format;

using std::string;
using std::vector;
using std::list;
using std::map;
using std::max;
using std::min;
using std::max_element;
using std::min_element;
using std::function;

using std::shared_ptr;
using std::unique_ptr;

END_NS_STARFALL
