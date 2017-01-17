//
//  MainDepot.h on Sep 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "MainNS.h"
#include "../Models/KeyFrame.h"


BEGIN_NS_MOVINGINSECTS

class MainDepot {
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
    
    int localZOrderForActiveCard() const;
    int localZOrderForPassiveCard() const;
    
    KeyFrame keyForPrelude() const;
    KeyFrame keyForLeftExpr() const;
    KeyFrame keyForOperator() const;
    KeyFrame keyForRightExpr() const;
    KeyFrame keyForEquals() const;
    KeyFrame keyForQuestionMark() const;
    
    KeyFrame keyForGroupA() const;
    KeyFrame keyForGroupB() const;
    KeyFrame keyForCardAppear() const;
    KeyFrame keyForCardOpen() const;

    void preloadSoundEffects() const;
    SoundEffect soundForCardinalNumber(int N) const;
    SoundEffect soundForOperator(const string& Op) const;
    SoundEffect soundForCardBirth() const;
    SoundEffect soundForCardDeath() const;
    SoundEffect soundForCardHit(int N = -1) const;
    SoundEffect soundForCardMiss() const;
    
private:
    float unitTime() const;
    vector<SoundEffect> soundEffectsToPreload() const;
};

END_NS_MOVINGINSECTS

