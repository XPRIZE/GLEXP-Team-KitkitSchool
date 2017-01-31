//
//  MainNS.h -- Namespace management for TutorialTrace
//  TodoSchool on Dec 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#define BEGIN_NS_TUTORIALTRACE \
namespace todoschool { \
namespace tutorialtrace {

#define END_NS_TUTORIALTRACE \
} \
}

#include <Games/NumberTrace/Common/ADT/Chain.h>
#include <Games/NumberTrace/Common/ADT/Optional.h>
#include <Games/NumberTrace/Common/ADT/TaggedFunction.h>
#include <Games/NumberTrace/Common/ADT/FunctionVector.h>
#include <Games/NumberTrace/Common/Basic/AARect.h>
#include <Games/NumberTrace/Common/Basic/Button.h>
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/NodeScopeGuard.h>
#include <Games/NumberTrace/Common/Basic/TodoSchoolDrawNode.h>
#include <Games/NumberTrace/Common/Repr/AllRepr.h>
#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/ProgressIndicator.hpp>
#include <Common/Controls/CompletePopup.hpp>
#include <Utils/TodoUtil.h>
#include <Utils/Random.h>

#include <cocos/cocos2d.h>
#include <cocos/ui/UIWidget.h>
#include <cocos/ui/UIButton.h>
#include <cocos/ui/UILayout.h>
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


BEGIN_NS_TUTORIALTRACE


// NB(xenosoz, 2016): Modify CREATE_FUNC to accept __TYPE__::create(x, y, z ...).
//   It calls __TYPE__::init(x, y, z ...) internally.
#ifdef CREATE_FUNC
#undef CREATE_FUNC
#endif

#define CREATE_FUNC(__TYPE__) \
template <typename... ArgTypes> \
static __TYPE__* create(ArgTypes&&... Args) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(std::forward<ArgTypes>(Args)...)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}


std::string itos(int I);
int stoi(const std::string& S);

using cocos2d::Vec2;
using cocos2d::Point;
using cocos2d::Size;
using Rect = todoschool::AARect;  // NB(xenosoz, 2016): Replacing cocos2d::Rect;
using cocos2d::Color3B;
using cocos2d::Color4B;
using cocos2d::BlendFunc;

using cocos2d::Node;
using cocos2d::ClippingNode;
using cocos2d::Label;
using cocos2d::Sprite;
using cocos2d::Camera;
using DrawNode = todoschool::TodoSchoolDrawNode;  // NB(xenosoz, 2016): Replacing cocos2d::DrawNode
using cocos2d::ui::Widget;
using cocos2d::ui::Layout;
using todoschool::Button;  // NB(xenosoz, 2016): Replacing cocos2d::ui::Button

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
using cocos2d::Hide;
using cocos2d::Show;
using cocos2d::TargetedAction;

using cocos2d::Vector;
using cocos2d::random;
using cocos2d::StringUtils::format;

using std::string;
using std::array;
using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::set;
using std::find;
using std::pair;
using std::tie;

using std::istream;
using std::ostream;
using std::stringstream;

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

END_NS_TUTORIALTRACE

