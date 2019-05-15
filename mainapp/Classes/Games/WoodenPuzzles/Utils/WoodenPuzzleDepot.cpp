//
//  WoodenPuzzleDepot.cpp on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzleDepot.h"
#include "WoodenPuzzleNS.h"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>
#include "Common/Sounds/CommonSound.hpp"
#include <string>

BEGIN_NS_WOODENPUZZLES;


string WoodenPuzzleDepot::assetPrefix() const {
    return mode;
}

string WoodenPuzzleDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size WoodenPuzzleDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size WoodenPuzzleDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Size WoodenPuzzleDepot::cardSize() const {
    return Size(504.f, 672.f);
}

float WoodenPuzzleDepot::scaleToCover(Size FixedSize, Size SubjectSize) const {
    float Scale = max(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float WoodenPuzzleDepot::scaleToBeContained(Size FixedSize, Size SubjectSize) const {
    float Scale = min(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float WoodenPuzzleDepot::scaleToCoverWindow(Size SubjectSize) const {
    return scaleToCover(windowSize(), SubjectSize);
}

float WoodenPuzzleDepot::scaleToBeContainedInWindow(Size SubjectSize) const {
    return scaleToBeContained(windowSize(), SubjectSize);
}

void WoodenPuzzleDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect WoodenPuzzleDepot::soundForCardinalNumber(int N) const {
    
    return CommonSound().cardinalNumber(N).preloaded();
}

SoundEffect WoodenPuzzleDepot::soundForLetter(const string& L) const {
    return CommonSound().soundForLetterName(L).preloaded();
}

SoundEffect WoodenPuzzleDepot::soundForCardBirth() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_birth.m4a");
}



SoundEffect WoodenPuzzleDepot::soundForCardHit(int N /* = -1 */) const {
    if (N < 0)
        N = random(0, 5);
    
    return SoundEffect(assetPrefix() + StringUtils::format("/Sounds/card_hit.%d.m4a", N));
}

SoundEffect WoodenPuzzleDepot::soundForCardMiss() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_miss.m4a");
}


vector<SoundEffect> WoodenPuzzleDepot::soundEffectsToPreload() const {
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

int WoodenPuzzleDepot::actionTagForPieceCreation() const {
    return 100;
}

int WoodenPuzzleDepot::actionTagForPullPiece() const {
    return 101;
}

WoodenPuzzleDepot::WoodenPuzzleDepot(const string& mode_)
: mode(mode_)
{
    bool valid = false;
    if (mode == "AlphabetPuzzle") { valid = true; }
    if (mode == "NumberPuzzle") { valid = true; }
    
    if (!valid) {
        // XXX
        throw std::invalid_argument(mode_);
    }
}


END_NS_WOODENPUZZLES;
