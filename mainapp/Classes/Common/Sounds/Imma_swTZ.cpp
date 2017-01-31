//
//  Imma_swTZ.cpp -- Rich voice of a female adult
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Imma_swTZ.h"
#include <cocos/deprecated/CCString.h>
#include <Games/NumberTrace/Common/Utils/StringUtils.h>
#include <string>


BEGIN_NS_TODOSCHOOL

using std::string;
using todoschool::toLower;
using cocos2d::StringUtils::format;


namespace {
    string assetPrefix() { return "Common/Sounds/Imma.sw_TZ"; }
}  // unnamed namespace

SoundEffect Imma_swTZ::soundForLetterName(const std::string& Letter) const {
    auto L = toLower(Letter);
    return SoundEffect(assetPrefix() + format("/LetterNames/%s.wav", L.c_str()));
}

SoundEffect Imma_swTZ::soundForWord(const std::string& Word) const {
    return SoundEffect(assetPrefix() + format("/Words/%s.wav", Word.c_str()));
}


END_NS_TODOSCHOOL
