//
//  SoundEffect.cpp on Aug 1, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SoundEffect.h"
#include <Managers/GameSoundManager.h>
#include <cocos/platform/CCFileUtils.h>
#include <string>



using cocos2d::FileUtils;
using std::string;


namespace {
    bool fileExist(const string& FilePath) {
        return cocos2d::FileUtils::getInstance()->isFileExist(FilePath);
    }
}  // unnamed namespace


SoundEffect::SoundEffect()
    : SoundEffect("")
{
}

SoundEffect::SoundEffect(const std::string& EffectPath)
    : EffectPath(EffectPath)
    , SoundID(-1)
    , Bad(false)
{
    if (!EffectPath.empty() && !fileExist(EffectPath)) {
        // NB(xenosoz, 2016): Check for missing sound effects
        CCLOGERROR("File does not exist for SoundEffect: %s", EffectPath.c_str());
        Bad = true;
    }
}

void SoundEffect::setPath(const std::string &path)
{
    EffectPath = path;
    SoundID = -1;
    Bad = false;
    
    if (!EffectPath.empty() && !fileExist(EffectPath)) {
        // NB(xenosoz, 2016): Check for missing sound effects
        CCLOGERROR("File does not exist for SoundEffect: %s", EffectPath.c_str());
        Bad = true;
    }
}


SoundEffect::operator bool() const {
    return !empty() && !bad();
}

bool SoundEffect::empty() const {
    return EffectPath.empty();
}

bool SoundEffect::bad() const {
    return Bad;
}

string SoundEffect::effectPath() const {
    return EffectPath;
}

void SoundEffect::preload() {
    GameSoundManager::getInstance()->preloadEffect(EffectPath);
}

void SoundEffect::play() {
    SoundID = GameSoundManager::getInstance()->playEffectSound(EffectPath);
}
    
void SoundEffect::playLoop() {
    SoundID = GameSoundManager::getInstance()->playEffectSound(EffectPath, true);
}

void SoundEffect::stop() {
    if (SoundID == (unsigned int)-1) { return; }
    GameSoundManager::getInstance()->stopEffectSound(SoundID);
}
    




SoundEffect& SoundEffect::preloaded() {
    preload();
    return *this;
}

SoundEffect& SoundEffect::operator&&(SoundEffect& RHS) {
    auto& LHS = *this;
    return (!LHS ? LHS : RHS);
}

SoundEffect& SoundEffect::operator||(SoundEffect& RHS) {
    auto& LHS = *this;
    return (LHS ? LHS : RHS);
}

const SoundEffect& SoundEffect::operator&&(const SoundEffect& RHS) const {
    auto& LHS = *this;
    return (!LHS ? LHS : RHS);
}

const SoundEffect& SoundEffect::operator||(const SoundEffect& RHS) const {
    auto& LHS = *this;
    return (LHS ? LHS : RHS);
}


