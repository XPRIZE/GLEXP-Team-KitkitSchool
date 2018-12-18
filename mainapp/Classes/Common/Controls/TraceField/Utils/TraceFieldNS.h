//
//  TraceFieldNS.h -- Namespace management for TraceField
//  TodoSchool on Sep 8, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#define BEGIN_NS_TRACEFIELD \
namespace todoschool { \
namespace tracefield {

#define END_NS_TRACEFIELD \
} \
}

#include <Common/ADT/Chain.h>
#include <Common/ADT/TaggedFunction.h>
#include <Common/ADT/FunctionVector.h>
#include "Common/Basic/AARect.h"
//#include <Games/NumberTrace/Common/Basic/Button.h>
#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/TodoSchoolDrawNode.h"
#include "Common/Repr/AllRepr.h"

#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/CompletePopup.hpp>
#include <Utils/TodoUtil.h>
//#include <Utils/Random.h>

#include <cocos/cocos2d.h>
#include <cocos/ui/UIWidget.h>
#include <cocos/ui/UIButton.h>
#include <cocos/ui/UILayout.h>
#include <cocos/ui/UIScrollView.h>
#include <3rdParty/json/json.h>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <limits>
#include <cmath>
#include <sstream>


BEGIN_NS_TRACEFIELD

std::string itos(int I);
int stoi(const std::string& S);

using cocos2d::Vec2;
using cocos2d::Point;
using cocos2d::Size;
//using Rect = todoschool::AARect;  // NB(xenosoz, 2016): Replacing cocos2d::Rect;
using cocos2d::Color3B;
using cocos2d::Color4B;
using cocos2d::BlendFunc;

using cocos2d::Node;
using cocos2d::Label;
using cocos2d::Sprite;
using cocos2d::Camera;
//using DrawNode = todoschool::TodoSchoolDrawNode;  // NB(xenosoz, 2016): Replacing cocos2d::DrawNode.
using cocos2d::ui::Widget;
using cocos2d::ui::Layout;
using cocos2d::ui::ScrollView;
using cocos2d::ui::Button;

using cocos2d::Component;
using cocos2d::EventListener;

using cocos2d::FiniteTimeAction;
using cocos2d::Sequence;
using cocos2d::Spawn;
using cocos2d::DelayTime;
using cocos2d::CallFunc;
using cocos2d::MoveBy;
using cocos2d::MoveTo;
using cocos2d::BezierBy;
using cocos2d::BezierTo;
using cocos2d::RotateBy;
using cocos2d::RotateTo;
using cocos2d::EaseIn;
using cocos2d::EaseOut;
using cocos2d::FadeIn;
using cocos2d::FadeOut;
using cocos2d::FadeTo;

using cocos2d::Vector;
using cocos2d::RefPtr;
using cocos2d::random;
using cocos2d::StringUtils::format;

using std::string;
using std::stringstream;
using std::array;
using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::set;
using std::find;
using std::pair;
using std::tie;

using std::max;
using std::min;
using std::max_element;
using std::min_element;
using std::count;
using std::count_if;
using std::exp;

using std::function;
using std::bind;
using std::numeric_limits;

using std::shared_ptr;
using std::unique_ptr;

END_NS_TRACEFIELD
