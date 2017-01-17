//
//  MainDepot.h on Jul 15, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Games/NumberTrace/Common/Basic/BigLabel.h>
#include <Common/Basic/SoundEffect.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <string>


namespace todoschool {
namespace wordtrace {

class MainDepot {
public:
    std::string assetPrefix() const;
    std::string defaultFont() const;
    cocos2d::Size windowSize() const;
    cocos2d::Size gameSize() const;
    
    cocos2d::Sprite* createBackgroundSprite() const;
    cocos2d::Node* createGameNode() const;

    BigLabel* createBonusTextLabel(const std::string& Text) const;
    cocos2d::Label* createNextLabel() const;
    cocos2d::ui::Button* createNextButton(const std::string& TeaserFileName) const;
    cocos2d::Label* createNextButtonLabel(const std::string& Text) const;
    
    void preloadSoundEffects() const;
    SoundEffect soundForWord(const std::string& Word) const;
    SoundEffect soundForGoodAssetTouch() const;
    SoundEffect soundForWorkComplete() const;
    
    std::string videoFilePathByFileName(const std::string& FileName) const;
    
private:
    cocos2d::Sprite* createNextButtonBottomSprite() const;
    cocos2d::Sprite* createNextButtonTopSprite() const;
    cocos2d::Sprite* createNextButtonEffect() const;
    cocos2d::Sprite* createNextButtonTeaserSprite(std::string TeaserFileName) const;
    
    std::vector<SoundEffect> soundEffectsToPreload() const;
};

}  // namespace wordtrace
}  // namespace todoschool