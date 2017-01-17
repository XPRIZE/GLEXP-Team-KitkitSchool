//
//  MainNS.h -- Namespace management for FindTheMatch
//  TodoSchool on Aug 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#define BEGIN_NS_FINDTHEMATCH \
    namespace todoschool { \
    namespace findthematch {

#define END_NS_FINDTHEMATCH \
    } \
    }

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <Games/NumberTrace/Common/ADT/Optional.h>
#include <Games/NumberTrace/Common/Basic/Button.h>
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include <Games/NumberTrace/Common/Repr/AllRepr.h>
#include <Common/Basic/SoundEffect.h>
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
#include <memory>
#include <iostream>
#include <sstream>


BEGIN_NS_FINDTHEMATCH

std::string itos(int I);
int stoi(const std::string& S);

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
using todoschool::Button;  // NB(xenosoz, 2016): Replacing cocos2d::ui::Button
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
using cocos2d::RotateBy;
using cocos2d::RotateTo;
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

using std::stringstream;
using std::istream;
using std::ostream;

END_NS_FINDTHEMATCH