//
//  MainDepot.cpp -- Resource management for FindTheMatch
//  TodoSchool on Aug 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "FindTheMatchMainDepot.h"
#include "FindTheMatchNS.h"
#include "Common/Basic/DeviceSpec.h"
#include <string>

BEGIN_NS_FINDTHEMATCH


std::string MainDepot::gameName() const {
    return "FindTheMatch";
}

std::string MainDepot::assetPrefix() const {
    return "FindTheMatch";
}

std::string MainDepot::defaultFont() const {
    return "fonts/mukta-bold.ttf";
}

Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Size MainDepot::cardSize() const {
    return Size(504.f, 672.f);
}

float MainDepot::scaleToCover(Size FixedSize, Size SubjectSize) const {
    float Scale = max(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float MainDepot::scaleToBeContained(Size FixedSize, Size SubjectSize) const {
    float Scale = min(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float MainDepot::scaleToCoverWindow(Size SubjectSize) const {
    return scaleToCover(windowSize(), SubjectSize);
}

float MainDepot::scaleToBeContainedInWindow(Size SubjectSize) const {
    return scaleToBeContained(windowSize(), SubjectSize);
}

int MainDepot::localZOrderForActiveCard() const {
    return 10;
}

int MainDepot::localZOrderForPassiveCard() const {
    return 0;
}

void MainDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect MainDepot::soundForCardBirth() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_birth.m4a");
}

SoundEffect MainDepot::soundForCardDeath() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_death.m4a");
}

SoundEffect MainDepot::soundForCardHit(int N /* = -1 */) const {
    if (N < 0)
        N = random(0, 5);
    
    return SoundEffect(assetPrefix() + StringUtils::format("/Sounds/card_hit.%d.m4a", N));
}

SoundEffect MainDepot::soundForCardMiss() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_miss.m4a");
}


vector<SoundEffect> MainDepot::soundEffectsToPreload() const {
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



END_NS_FINDTHEMATCH
