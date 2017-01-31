//
//  SpellingDepot.cpp on Aug 10, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellingDepot.h"
#include "SpellingNS.h"
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <string>

BEGIN_NS_SPELLING;


std::string SpellingDepot::assetPrefix() const {
    return "OldSpelling";
}

std::string SpellingDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size SpellingDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size SpellingDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

float SpellingDepot::scaleToCover(Size FixedSize, Size SubjectSize) const {
    float Scale = max(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float SpellingDepot::scaleToBeContained(Size FixedSize, Size SubjectSize) const {
    float Scale = min(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float SpellingDepot::scaleToCoverWindow(Size SubjectSize) const {
    return scaleToCover(windowSize(), SubjectSize);
}

float SpellingDepot::scaleToBeContainedInWindow(Size SubjectSize) const {
    return scaleToBeContained(windowSize(), SubjectSize);
}

void SpellingDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect SpellingDepot::soundForCardBirth() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_birth.m4a");
}

SoundEffect SpellingDepot::soundForCardDeath() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_death.m4a");
}

SoundEffect SpellingDepot::soundForCardHit(int N /* = -1 */) const {
    if (N < 0)
        N = random(0, 5);
    
    return SoundEffect(assetPrefix() + StringUtils::format("/Sounds/card_hit.%d.m4a", N));
}

SoundEffect SpellingDepot::soundForCardMiss() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_miss.m4a");
}


vector<SoundEffect> SpellingDepot::soundEffectsToPreload() const {
    return {
        soundForCardBirth(),
        soundForCardHit(0),
        soundForCardHit(1),
        soundForCardHit(2),
        soundForCardHit(3),
        soundForCardHit(4),
        soundForCardHit(5),
        soundForCardMiss(),
    };
}


END_NS_SPELLING;
