//
//  MainDepot.cpp -- Basic resource management for Spelling
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"

#include "Common/Sounds/CommonSound.hpp"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>


BEGIN_NS_SPELLING

string MainDepot::assetPrefix() const {
    return "Spelling";
}

string MainDepot::defaultFont() const {
    return "fonts/Andika-R.ttf";
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

vector<string> MainDepot::candidateLetterChoices() const {
    if (LanguageManager::getInstance()->isSwahili()) {
        // NB(xenosoz, 2016): We remove "q", "x" in Swahili.
        return vector<string>{
            "a", "b", "c", "d", "e", "f", "g",
            "h", "i", "j", "k", "l", "m", "n",
            "o", "p",      "r", "s", "t", "u",
            "v", "w",      "y", "z",
        };
    }

    return vector<string>{
        "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s", "t", "u",
        "v", "w", "x", "y", "z",
    };
}

vector<string> MainDepot::auxiliaryConsonantsFor(const string& Answer) const {
    string Consonants = "bcdfghjklmnpqrstvwxyz";

    if (LanguageManager::getInstance()->isSwahili()) {
        // NB(xenosoz, 2016): We remove "q", "x" in Swahili.
        Consonants = string("bcdfghjklmnprstvwyz");
    }

    vector<string> It;
    for (auto C : Consonants) {
        if (find(Answer.begin(), Answer.end(), C) != Answer.end()) { continue; }
        It.push_back(string{C});
    }
        
    return It;
}

vector<string> MainDepot::auxiliaryVowelsFor(const string& Answer) const {
    vector<string> It;
    for (auto V : string("aeiou")) {
        if (find(Answer.begin(), Answer.end(), V) != Answer.end()) { continue; }
        It.push_back(string{V});
    }
    
    return It;
}

bool MainDepot::allowFadeInOutForGameBoard() const {
    return false;
}

bool MainDepot::allowFadeInOutForAnswerBall() const {
    return false;
}

SoundEffect MainDepot::soundForLetterName(const string& Letter) const {
    if (Letter.empty()) { return SoundEffect::emptyEffect(); }
    
    return CommonSound().soundForLetterName(Letter).preloaded();
    
}

SoundEffect MainDepot::soundForWord(const string& Word) const {
    if (Word.empty()) { return SoundEffect::emptyEffect(); }
    
    return CommonSound().soundForWord(Word).preloaded();
        
}

SoundEffect MainDepot::soundForCardBirth() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_birth.m4a").preloaded();
}

SoundEffect MainDepot::soundForCardDeath() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_death.m4a").preloaded();
}

SoundEffect MainDepot::soundForCardHit(int N /* = -1 */) const {
    if (N < 0)
        N = random(0, 5);

    string FilePath = assetPrefix() + StringUtils::format("/Sounds/card_hit.%d.m4a", N);
    return SoundEffect(FilePath).preloaded();
}

SoundEffect MainDepot::soundForCardMiss() const {
    return SoundEffect(assetPrefix() + "/Sounds/card_miss.m4a").preloaded();
}

END_NS_SPELLING


