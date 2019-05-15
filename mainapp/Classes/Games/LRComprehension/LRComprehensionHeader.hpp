//
//  LRComprehensionHeader.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 8/25/18.
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

#include "LRGuideView.hpp"

#define LOGFN()\
CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_LR_COMPREHENSION \
namespace LRComprehension {\

#define END_NS_LR_COMPREHENSION \
}

// ====================================================================================================

BEGIN_NS_LR_COMPREHENSION;

enum class LRComprehensionType
{
    None,
    TitlePage,
    LCAllInOne,
    RCAllInOne,
    ListeningOnly,
    ReadingOnly,
    MultipleChoices,
    FillTheBlank,
    Ordering,
};

static Size fixedDeviceSize = Size(2560, 1800);

END_NS_LR_COMPREHENSION;

