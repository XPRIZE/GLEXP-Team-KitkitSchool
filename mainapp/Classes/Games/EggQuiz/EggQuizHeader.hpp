//
//  EggQuizHeader.hpp
//  KitkitSchool
//
//  Created by Jaehun Jung on 29/08/2018.
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

#define BEGIN_NS_EGGQUIZ \
namespace EggQuiz {

#define END_NS_EGGQUIZ \
}

BEGIN_NS_EGGQUIZ;

const string partsPath = "eggquiz/parts/";
const string imagesPath = "eggquiz/images/";
const string soundsPath = "eggquiz/sounds/";
const string soundsNumberPath = "numbervoice/d_";
const string tsvPath = "eggquiz/";

const string FONT_NORMAL = "fonts/mukta-bold.ttf";
const string FONT_BOLD = "fonts/chanakya.ttf";
const Color4B FONT_COLOR = Color4B(60,60,60,255);

const Size BUTTON_SIZE_SMALL = Size(192, 300);

enum class SlotType {
    Normal,
    LinkedNumber,
    Sentence
};

enum class SlotSize {
    Small,
    Big,
    Bar
};
const Size SLOT_SIZE_SMALL = Size(204, 312);

enum class SpeakerSize {
    Small,
    Big
};
const float autoStartDelay = 0.7;

const Size SlotSizeBar = Size(2012, 152);
const Size SlotSizeSmall = Size(204, 312);

enum class missingNumberMode {
    Normal,
    Expanded
};

END_NS_EGGQUIZ;
