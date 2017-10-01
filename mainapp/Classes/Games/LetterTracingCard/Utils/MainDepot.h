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
namespace lettertracingcard {

class MainDepot {
public:
    std::string assetPrefix() const;
    std::string defaultFont() const;
    cocos2d::Size windowSize() const;
    cocos2d::Size gameSize() const;
    
    cocos2d::Sprite* createBackgroundSprite() const;
    cocos2d::Node* createGameNode() const;
};

}  // namespace lettertracingcard
}  // namespace todoschool
