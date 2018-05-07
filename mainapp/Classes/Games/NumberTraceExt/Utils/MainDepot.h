//
//  MainDepot.h on Jul 15, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "Common/Basic/BigLabel.h"
#include <Common/Basic/SoundEffect.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <string>


namespace todoschool {
namespace numbertraceext {

class MainDepot {
public:
    std::string assetPrefix() const;
    std::string defaultFont() const;
    cocos2d::Size windowSize() const;
    cocos2d::Size gameSize() const;
    
    cocos2d::Sprite* createBackgroundSprite() const;
    cocos2d::Node* createGameNode() const;

    SoundEffect soundForTraceEnd() const;
    
private:


    
    std::vector<SoundEffect> soundEffectsToPreload() const;
};

}  // namespace numbertraceext
}  // namespace todoschool
