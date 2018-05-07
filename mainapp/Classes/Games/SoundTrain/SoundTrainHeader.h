//
//  SoundTrainHeader.h
//  KitkitSchool
//
//  Created by JungJaehun on 31/08/2017.
//
//
#pragma once

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>
#include <string>
#include <functional>
#include <algorithm>

using namespace cocos2d::ui;
using namespace std;

#include "3rdParty/json/json.h"
#include "Utils/TodoUtil.h"

#include "Managers/LanguageManager.hpp"

#define LOGFN()		CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_SOUNDTRAIN \
namespace todoschool { \
namespace soundtrain {

#define END_NS_SOUNDTRAIN \
} \
}
