//
//  LRDividedTypeReorderingHeader.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 8/19/18.
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <algorithm>

using namespace cocos2d::ui;
using namespace std;

#include "3rdParty/json/json.h"
#include "3rdParty/CCNativeAlert.h"
#include "Utils/TodoUtil.h"
#include "Managers/GameSoundManager.h"
#include "Managers/LanguageManager.hpp"
#include "Managers/StrictLogManager.h"

#define LOGFN()\
CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_LR_DIVIDED_TYPE_REORDERING \
namespace LRComprehension {\
namespace LRDividedTypeReordering {

#define END_NS_LR_DIVIDED_TYPE_REORDERING \
}\
}

// ====================================================================================================

BEGIN_NS_LR_DIVIDED_TYPE_REORDERING;

enum class GuideType
{
    Text = 0,
    Sound,
};

enum class GameType
{
    None = -1,
    Image,
    Text,
};

static Size fixedDeviceSize = Size(2560, 1800);

END_NS_LR_DIVIDED_TYPE_REORDERING;
