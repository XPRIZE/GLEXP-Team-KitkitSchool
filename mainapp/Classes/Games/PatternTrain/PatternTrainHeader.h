//
//  PatternTrainHeader.h
//  enumaXprize
//
//  Created by JungJaehun on 08/09/2017.
//
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

#define LOGFN()		CCLOG("=========== %s ===========", __PRETTY_FUNCTION__)

#define BEGIN_NS_PATTERNTRAIN \
namespace todoschool { \
namespace patterntrain {

#define END_NS_PATTERNTRAIN \
} \
}
