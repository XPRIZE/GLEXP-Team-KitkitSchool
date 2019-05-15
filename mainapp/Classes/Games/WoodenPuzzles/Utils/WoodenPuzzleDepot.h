//
//  WoodenPuzzleDepot.h on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "WoodenPuzzleNS.h"

BEGIN_NS_WOODENPUZZLES;


class WoodenPuzzleDepot {
public:
    string assetPrefix() const;
    string defaultFont() const;

    Size windowSize() const;
    Size gameSize() const;
    Size cardSize() const;

    float scaleToCover(Size FixedSize, Size SubjectSize) const;
    float scaleToBeContained(Size FixedSize, Size SubjectSize) const;
    float scaleToCoverWindow(Size SubjectSize) const;
    float scaleToBeContainedInWindow(Size SubjectSize) const;
    
    // NB(xenosoz, 2016): I'm trying new style. Preload on creation.
    SoundEffect soundForCardinalNumber(int N) const;
    SoundEffect soundForLetter(const string& L) const;

    void preloadSoundEffects() const;
    SoundEffect soundForCardBirth() const;
    SoundEffect soundForCardHit(int N = -1) const;
    SoundEffect soundForCardMiss() const;
    
    int actionTagForPieceCreation() const;
    int actionTagForPullPiece() const;

public:
    WoodenPuzzleDepot(const string& mode);
    
private:
    vector<SoundEffect> soundEffectsToPreload() const;
    string mode;
};


END_NS_WOODENPUZZLES;
