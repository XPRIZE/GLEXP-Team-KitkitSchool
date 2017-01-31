//
//  SpellingDepot.h on Aug 10, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "SpellingNS.h"


BEGIN_NS_SPELLING;

class SpellingDepot {
public:
    string assetPrefix() const;
    string defaultFont() const;

    Size windowSize() const;
    Size gameSize() const;

    float scaleToCover(Size FixedSize, Size SubjectSize) const;
    float scaleToBeContained(Size FixedSize, Size SubjectSize) const;
    float scaleToCoverWindow(Size SubjectSize) const;
    float scaleToBeContainedInWindow(Size SubjectSize) const;
    
    void preloadSoundEffects() const;
    SoundEffect soundForCardBirth() const;
    SoundEffect soundForCardDeath() const;
    SoundEffect soundForCardHit(int N = -1) const;
    SoundEffect soundForCardMiss() const;
    
private:
    vector<SoundEffect> soundEffectsToPreload() const;
};

END_NS_SPELLING;