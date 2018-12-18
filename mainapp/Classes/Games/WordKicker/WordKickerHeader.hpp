//
//  WordKickerHeader.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/14/18.
//

#pragma once

#ifndef WordKickerHeader_hpp
#define WordKickerHeader_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common/Basic/SoundEffect.h"

#include "Managers/LanguageManager.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/GameSoundManager.h"
#include "Managers/StrictLogManager.h"

#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <functional>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;


#define BEGIN_NS_WORDKICKER \
namespace WordKicker { \

#define END_NS_WORDKICKER \
}




BEGIN_NS_WORDKICKER;

struct WordKickerProblemStruct
{
	string Sentence;
	string SentenceSound;
	string Word1;
	string Word2;
	string Word3;
	int Time;

	string Answer;
	int AnswerStartPos;
	int AnswerLength;
	
	int multiLineStartPos;
};

static string gameName = "WordKicker";
static string resourcePath = "WordKicker/";

const Size gameSize = Size(2560, 1800);

//const string fontName = "fonts/TodoSchoolV2.ttf";
//const string fontName = "fonts/AndikaNewBasic-B.ttf";
const string fontName = "fonts/andika-r.ttf";
const float fontSize_normal = 120;
const float fontSize_math = 180;
const bool gameDebug = false;
const Color4B colorLabel = Color4B(49, 38, 38, 255);
const Color4B colorAnswerLabel = Color4B(0, 138, 230, 255);
const Color4B colorTimeoverAnswerLabel = Color4B(242, 5, 0, 255);

const float kicker_x = -830;
const float kicker_y = -372;

const float goalkeeper_x = 228;
const float goalkeeper_y = -372;

//const float ball_x = -527;
const float ball_x = -600;
const float ball_y = -363;

//extern SoundEffect pickEffect();

END_NS_WORDKICKER;


#endif /* WordKickerHeader_hpp */
