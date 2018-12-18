//
//  MainDepot.h -- Resource management for FindTheMatch
//  TodoSchool on Aug 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "FindTheMatchNS.h"

BEGIN_NS_FINDTHEMATCH


class MainDepot {
public:
    string gameName() const;
    string assetPrefix() const;
    string defaultFont() const;

    Size windowSize() const;
    Size gameSize() const;
    Size cardSize() const;

    float scaleToCover(Size FixedSize, Size SubjectSize) const;
    float scaleToBeContained(Size FixedSize, Size SubjectSize) const;
    float scaleToCoverWindow(Size SubjectSize) const;
    float scaleToBeContainedInWindow(Size SubjectSize) const;
    
    int localZOrderForActiveCard() const;
    int localZOrderForPassiveCard() const;

    void preloadSoundEffects() const;
    SoundEffect soundForCardBirth() const;
    SoundEffect soundForCardDeath() const;
    SoundEffect soundForCardHit(int N = -1) const;
    SoundEffect soundForCardMiss() const;
    
private:
    vector<SoundEffect> soundEffectsToPreload() const;
};

END_NS_FINDTHEMATCH
