//
//  EggQuizMathHeader.h
//  KitkitSchool
//
//  Created by JungJaehun on 26/03/2018.
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
#include "Utils/TodoUtil.h"

#define LOGFN()        CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_EGGQUIZMATH \
namespace EggQuizMath {

#define END_NS_EGGQUIZMATH \
}
