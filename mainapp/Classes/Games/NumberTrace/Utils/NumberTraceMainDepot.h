//
//  MainDepot.h on Apr 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "NumberTraceNS.h"
#include "../Models/NumberTraceProblem.h"
#include <Common/Basic/SoundEffect.h>
#include <cocos/cocos2d.h>
#include <vector>
#include <string>


BEGIN_NS_NUMBERTRACE

class MainDepot {
public:
    string assetPrefix() const;    
    Size windowSize() const;
    Size gameSize() const;

    Sprite* createBackgroundSprite() const;
    Node* createGameNode() const;
    
    void preloadSoundEffects() const;
    SoundEffect soundForNumber(int Number) const;
    SoundEffect soundForTraceEnd() const;
    SoundEffect soundForGoodAssetTouch() const;
    SoundEffect soundForBadAssetTouch() const;
    SoundEffect soundForWorkComplete() const;

    Problem::AssetType assetTypeForString(const string& S);
    string stringForAssetType(Problem::AssetType T);
    
private:
    vector<SoundEffect> soundEffectsToPreload() const;
    
    SoundEffect englishSoundForNumber(int Number) const;
    SoundEffect swahiliSoundForNumber(int Number) const;
};

END_NS_NUMBERTRACE
