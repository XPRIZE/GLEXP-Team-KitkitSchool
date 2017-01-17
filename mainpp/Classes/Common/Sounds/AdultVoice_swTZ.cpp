//
//  AdultVoice_swTZ.cpp -- Rich voice of a female adult
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "AdultVoice_swTZ.h"
#include <cocos/deprecated/CCString.h>
#include <string>


BEGIN_NS_TODOSCHOOL

using std::string;
using cocos2d::StringUtils::format;


namespace {
    string assetPrefix() { return "Common/AdultVoice/sw"; }
}  // unnamed namespace


SoundEffect AdultVoice_swTZ::cardinalNumber(int N) const {
    if (1 <= N && N <= 30)
        return SoundEffect(assetPrefix() + format("/D_%d.m4a", N));
    
    return SoundEffect::emptyEffect();
}

SoundEffect AdultVoice_swTZ::equals() const {
    return SoundEffect(assetPrefix() + "/Equals.m4a");
}

SoundEffect AdultVoice_swTZ::minus() const {
    return SoundEffect(assetPrefix() + "/Minus.m4a");
}

SoundEffect AdultVoice_swTZ::plus() const {
    return SoundEffect(assetPrefix() + "/Plus.m4a");
}

END_NS_TODOSCHOOL