//
//  Pam_enUS.cpp -- A voice of a female adult
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Pam_enUS.h"
#include <cocos/deprecated/CCString.h>
#include <string>
#include <cocos/platform/CCFileUtils.h>

BEGIN_NS_TODOSCHOOL

using std::string;
using cocos2d::StringUtils::format;


namespace {
    string assetPrefix() { return "Common/Sounds/Pam.en_US"; }
}  // unnamed namespace

SoundEffect Pam_enUS::soundForLetterName(const std::string& Letter) const {
    if (Letter.size() == 1 && 'a' <= Letter[0] && Letter[0] <= 'z') {
        char C = Letter[0];
        return SoundEffect(assetPrefix() + format("/LetterNames/eng_%c.wav", C));
    }
    if (Letter.size() == 1 && 'A' <= Letter[0] && Letter[0] <= 'Z') {
        // NB(xenosoz, 2016): Convert [A-Z] to [a-z].
        char C = Letter[0] - 'A' + 'a';
        return SoundEffect(assetPrefix() + format("/LetterNames/eng_%c.wav", C));
    }
    
    return SoundEffect::emptyEffect();
}

SoundEffect Pam_enUS::soundForWord(const std::string& Word) const {
    auto path = assetPrefix() + format("/Words/%s.wav", Word.c_str());
    if (!cocos2d::FileUtils::getInstance()->isFileExist(path)) {
        path = assetPrefix() + format("/Words/%s.m4a", Word.c_str());
    }
    return SoundEffect(path.c_str());
}


END_NS_TODOSCHOOL
