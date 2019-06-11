//
//  SoundEffect.h on Aug 1, 2016 -- Basic sound effect
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once


#include <string>


class SoundEffect {
    std::string EffectPath;
    unsigned int SoundID;
    bool Bad;

public:
    // NB(xenosoz, 2016): Don't rely on SoundEffect() making a valid empty sound object.
    //   Use SoundEffect::emptyEffect() instead.
    SoundEffect();
    SoundEffect(const std::string& EffectPath);

    void setPath(const std::string& path);
    
    operator bool() const;
    bool empty() const;
    bool bad() const;

    std::string effectPath() const;

    void preload();
    void play();
    void playLoop();
   // void playTtsSound(const std::string& word);
    //void pause();  // XXX: Not implemented.
    void stop();
    //void unload();  // XXX: Not implemented.

    // NB(xenosoz, 2016): Sugars
    SoundEffect& preloaded();
    SoundEffect& operator&&(SoundEffect& RHS);
    SoundEffect& operator||(SoundEffect& RHS);
    const SoundEffect& operator&&(const SoundEffect& RHS) const;
    const SoundEffect& operator||(const SoundEffect& RHS) const;

public:
    static SoundEffect emptyEffect() { return SoundEffect(""); }
    
    
    static SoundEffect menuBackEffect() { return SoundEffect("Common/Sounds/Effect/SFX_GUIBack.m4a"); }
    static SoundEffect buttonEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Button.m4a"); }
    
    static SoundEffect coopClickEffect() { return SoundEffect("Common/Sounds/Effect/SFX_SelectHut.m4a"); }
    static SoundEffect eggClickEffect() { return SoundEffect("Common/Sounds/Effect/SFX_EggSelect.m4a"); }
    static SoundEffect iconSpawnEffect() { return SoundEffect("Common/Sounds/Effect/SFX_TitleBounceLand.m4a"); }
    static SoundEffect birdJumpEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Jump.m4a"); }
    static SoundEffect birdGrowEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Grow.m4a"); }
    static SoundEffect dayClearEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Fanfare1.m4a"); }
    static SoundEffect levelClearEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Fanfare2.m4a"); }
    static SoundEffect eggCrackEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Hatch.m4a"); }
    static SoundEffect eggHaloEffect() { return SoundEffect("Common/Sounds/Effect/SFX_TitleFall.m4a"); }
    static SoundEffect eggHatchEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Fanfare3.m4a"); }
    static SoundEffect particleEffect() { return SoundEffect("Common/Sounds/Effect/SFX_Twinkle.m4a"); }
    

    static SoundEffect wowEffect() { return SoundEffect("Common/Sounds/Effect/SFX_ChildrenCheerNew.m4a"); }
    
    static SoundEffect lightEffect() { return SoundEffect("Common/Sounds/Effect/Light_on.m4a"); }
    static SoundEffect starOutEffect() { return SoundEffect("Common/Sounds/Effect/UI_Treasure_Reward2.m4a"); }
    static SoundEffect startInEffect() { return SoundEffect("Common/Sounds/Effect/UI_StarAdded.m4a"); }
    

};


