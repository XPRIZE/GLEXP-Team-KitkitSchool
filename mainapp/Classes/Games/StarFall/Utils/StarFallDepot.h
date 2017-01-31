//
//  StarFallDepot.h on Jul 15, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "StarFallNS.h"
#include "../Models/Symbol.h"


BEGIN_NS_STARFALL

class StarFallDepot {
public:
    string assetPrefix() const;
    string defaultFontFace() const;
    Size windowSize() const;
    Size gameSize() const;
    
    Node* createBackgroundNode() const;
    Node* createGameNode() const;
    
    Json::Value jsonForMainGameWork();
    SymbolLayoutType defaultSymbolLayout() const;
    SymbolSetType defaultEnabledSymbolSet() const;
    
    float keyForBubblePop() const;
    float keyForBubbleScatter() const;

    void preloadSoundEffects() const;
    SoundEffect soundForKeyInput() const;
    SoundEffect soundForTextHit(int N = -1) const;
    SoundEffect soundForTextMiss() const;
    SoundEffect soundForTargetBirth() const;
    SoundEffect soundForTargetDeath() const;
    SoundEffect soundForWord(const string& Word) const;


private:
    Json::Value swahiliJsonForMainGameWork();
    Json::Value englishJsonForMainGameWork();

    vector<SoundEffect> soundEffectsToPreload() const;
};

END_NS_STARFALL
