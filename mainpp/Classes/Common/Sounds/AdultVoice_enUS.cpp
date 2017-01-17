//
//  AdultVoice_enUS.cpp -- Light voice of an adult male
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "AdultVoice_enUS.h"
#include <cocos/deprecated/CCString.h>
#include <string>


BEGIN_NS_TODOSCHOOL

using std::string;
using cocos2d::StringUtils::format;


namespace {
    string assetPrefix() { return "Common/AdultVoice/en"; }
}  // unnamed namespace


SoundEffect AdultVoice_enUS::cardinalNumber(int N) const {
    if (0 <= N && N <= 100)
        return SoundEffect(assetPrefix() + format("/D_%d.m4a", N));

    return SoundEffect::emptyEffect();
}

SoundEffect AdultVoice_enUS::equals() const {
    return SoundEffect(assetPrefix() + "/Equals.m4a");
}

SoundEffect AdultVoice_enUS::minus() const {
    return SoundEffect(assetPrefix() + "/Minus.m4a");
}

SoundEffect AdultVoice_enUS::plus() const {
    return SoundEffect(assetPrefix() + "/Plus.m4a");
}

SoundEffect AdultVoice_enUS::times() const {
    return SoundEffect(assetPrefix() + "/Times.m4a");
}

END_NS_TODOSCHOOL
