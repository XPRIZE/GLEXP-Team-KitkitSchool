//
//  LRDividedTypeFillTheBlanksHeader.hpp
//  KitkitSchool
//
//  Created by HyeonGyu Yu on 23/08/2018.
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

#include "Common/Controls/DottedRect.hpp"
#include "../LRSoundButton.hpp"

#define LOGFN()\
CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS \
namespace LRComprehension {\
namespace LRDividedTypeFillTheBlanks {

#define END_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS \
}\
}

// ====================================================================================================

BEGIN_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;

static Size fixedDeviceSize = Size(2560, 1800);

END_NS_LR_DIVIDED_TYPE_FILL_THE_BLANKS;
