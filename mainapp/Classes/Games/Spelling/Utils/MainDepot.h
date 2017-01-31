//
//  MainDepot.h -- Basic resource management for Spelling
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "MainNS.h"


BEGIN_NS_SPELLING

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
    
    vector<string> candidateLetterChoices() const;
    vector<string> auxiliaryConsonantsFor(const string& Answer) const;
    vector<string> auxiliaryVowelsFor(const string& Answer) const;
    bool allowFadeInOutForGameBoard() const;
    bool allowFadeInOutForAnswerBall() const;
    
    SoundEffect soundForLetterName(const string& Letter) const;
    SoundEffect soundForWord(const string& Word) const;
    SoundEffect soundForCardBirth() const;
    SoundEffect soundForCardDeath() const;
    SoundEffect soundForCardHit(int N = -1) const;
    SoundEffect soundForCardMiss() const;
};

END_NS_SPELLING


