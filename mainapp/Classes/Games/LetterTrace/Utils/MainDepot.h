//
//  MainDepot.h on Jun 30, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Common/Basic/SoundEffect.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <string>


namespace todoschool {
namespace lettertrace {

class MainDepot {
public:
    std::string assetPrefix() const;
    cocos2d::Size windowSize() const;
    cocos2d::Size gameSize() const;
    
    cocos2d::Sprite* createBackgroundSprite() const;
    cocos2d::Node* createGameNode() const;
    cocos2d::Label* createNextLabel() const;
    cocos2d::ui::Button* createNextButton(std::string T) const;

    void preloadSoundEffects() const;
    SoundEffect soundForLetter(const std::string& Letter) const;
    SoundEffect soundForWord(const std::string& Word) const;
    SoundEffect soundForLetterBetweenWords() const;
    SoundEffect soundForTraceEnd() const;
    SoundEffect soundForGoodAssetTouch() const;
    SoundEffect soundForBadAssetTouch() const;
    SoundEffect soundForWorkComplete() const;
    
    std::string videoFilePathByFileName(std::string FileName);
    
private:
    cocos2d::Sprite* createNextButtonBottomSprite() const;
    cocos2d::Sprite* createNextButtonTopSprite() const;
    cocos2d::Sprite* createNextButtonEffect() const;
    cocos2d::Sprite* createNextButtonTextSprite(const std::string& T) const;
    
    std::vector<SoundEffect> soundEffectsToPreload() const;
};

}  // namespace lettertrace
}  // namespace todoschool