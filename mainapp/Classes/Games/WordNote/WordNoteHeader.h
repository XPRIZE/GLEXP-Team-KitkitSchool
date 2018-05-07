//
//  WordNoteHeader.h
//  KitkitSchool
//
//  Created by JungJaehun on 21/02/2018.
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

#define BEGIN_NS_WORDNOTE \
namespace WordNote { \

#define END_NS_WORDNOTE \
}

BEGIN_NS_WORDNOTE;

const Size slotSingleSize = Size(220, 240);
const Size slotDoubleSize = Size(380, 240);
const Size slotTripleSize = Size(540, 240);
const Size slotQuadrupleSize = Size(700, 240);

END_NS_WORDNOTE;
