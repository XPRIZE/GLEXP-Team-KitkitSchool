//
//  SentenceBridgeHeader.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#pragma once

#ifndef SentenceBridgeHeader_hpp
#define SentenceBridgeHeader_hpp

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


#define BEGIN_NS_SENTENCEBRIDGE \
namespace SentenceBridge { \

#define END_NS_SENTENCEBRIDGE \
}




BEGIN_NS_SENTENCEBRIDGE;

struct SentenceBridgeProblemStruct
{
	string Sentence;
	string SentenceSound;
    bool UpperCase;
	vector<int> ProblemSentenceIndices;

	int multiLineStartPos;
    bool HasAlphabet;
};

const string resourcePath = "SentenceBridge/";

const Size gameSize = Size(2560, 1800);

//const string fontName = "fonts/TodoSchoolV2.ttf";
//const string fontName = "fonts/AndikaNewBasic-B.ttf";
const string fontName = "fonts/andika-r.ttf";
const float fontSize = 140;
const float fontSize_ch = 100;
const bool gameDebug = false;

//extern SoundEffect pickEffect();

END_NS_SENTENCEBRIDGE;


#endif /* SentenceBridgeHeader_hpp */
