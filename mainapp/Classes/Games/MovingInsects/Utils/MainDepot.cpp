//
//  MainDepot.cpp on Sep 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"
#include "MainNS.h"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>
#include "Common/Sounds/CommonSound.hpp"

#include <string>


BEGIN_NS_MOVINGINSECTS

string MainDepot::assetPrefix() const {
    return "MovingInsects";
}

string MainDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Size MainDepot::cardSize() const {
    return Size(218.f, 338.f);
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

KeyFrame MainDepot::keyForPrelude() const {
    return KeyFrame(unitTime() * 0, unitTime() * 1);
}

KeyFrame MainDepot::keyForLeftExpr() const {
    return KeyFrame(unitTime() * 1, unitTime() * 2);
}

KeyFrame MainDepot::keyForOperator() const {
    return KeyFrame(unitTime() * 2, unitTime() * 3);
}

KeyFrame MainDepot::keyForRightExpr() const {
    return KeyFrame(unitTime() * 3, unitTime() * 4);
}

KeyFrame MainDepot::keyForEquals() const {
    return KeyFrame(unitTime() * 4, unitTime() * 5);
}

KeyFrame MainDepot::keyForQuestionMark() const {
    return KeyFrame(unitTime() * 5, unitTime() * 6);
}

KeyFrame MainDepot::keyForGroupA() const {
    return KeyFrame(keyForPrelude().interpolate(0.30f), keyForLeftExpr().beginTime());
}

KeyFrame MainDepot::keyForGroupB() const {
    return KeyFrame(keyForOperator().interpolate(0.30f), keyForRightExpr().beginTime());
}

KeyFrame MainDepot::keyForCardAppear() const {
    return KeyFrame(keyForRightExpr().interpolate(0.30f), keyForEquals().beginTime());
}

KeyFrame MainDepot::keyForCardOpen() const {
    return KeyFrame(keyForEquals().interpolate(0.30f), keyForQuestionMark().beginTime());
}

void MainDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect MainDepot::soundForCardinalNumber(int N) const {
    SoundEffect SE = CommonSound().cardinalNumber(N);
    
    return SE;
}

SoundEffect MainDepot::soundForOperator(const string &Op) const {
    auto soundForPlus = [] {
        SoundEffect SE = CommonSound().plus();
        return SE;
    };

    auto soundForMinus = [] {
        SoundEffect SE = CommonSound().minus();
        return SE;
    };
    
    if (Op == "+") { return CommonSound().plus(); }
    if (Op == "-") { return CommonSound().minus(); }
    if (Op == "=") { return CommonSound().equals(); }
    
    return SoundEffect();
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


float MainDepot::unitTime() const {
    return 1.12f;
}

vector<SoundEffect> MainDepot::soundEffectsToPreload() const {
    return {
        soundForCardinalNumber(0),
        soundForCardinalNumber(1),
        soundForCardinalNumber(2),
        soundForCardinalNumber(3),
        soundForCardinalNumber(4),
        soundForCardinalNumber(5),
        soundForCardinalNumber(6),
        soundForCardinalNumber(7),
        soundForCardinalNumber(8),
        soundForCardinalNumber(9),
        
        soundForOperator("+"),
        soundForOperator("-"),
        soundForOperator("="),
        
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

END_NS_MOVINGINSECTS
