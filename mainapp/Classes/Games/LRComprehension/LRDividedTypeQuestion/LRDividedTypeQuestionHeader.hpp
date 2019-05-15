//
//  MultipleChoiceHeader.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 13/08/2018.
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

#include "../LRSoundButton.hpp"

#define LOGFN()\
CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_LR_DIVIDED_TYPE_QUESTION \
namespace LRComprehension {\
namespace LRDividedTypeQuestion {
    
#define END_NS_LR_DIVIDED_TYPE_QUESTION \
}\
}

// ====================================================================================================

BEGIN_NS_LR_DIVIDED_TYPE_QUESTION;

enum class GuideType
{
    Text = 0,
    Sound,
};

enum class QuestionType
{
    None = -1,
    TextAndText,
};

static Size fixedDeviceSize = Size(2560, 1800);

END_NS_LR_DIVIDED_TYPE_QUESTION;
