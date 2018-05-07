//
//  MainDepot.cpp on Jul 15, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"
#include "../../LetterTracingCard/Utils/MainDepot.h"
#include <Managers/LanguageManager.hpp>
#include "Common/Basic/DeviceSpec.h"
#include "Common/Repr/AllRepr.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


namespace todoschool {
namespace lettertracingcard {

std::string MainDepot::assetPrefix() const {
    return "LetterTracingCard";
}

std::string MainDepot::defaultFont() const {
    return "fonts/kitkitalphabet.ttf";
}

Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Sprite* MainDepot::createBackgroundSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/tracing-3_image_background.png");
    
    Size WindowSize = windowSize();
    Size SpriteSize = It->getContentSize();
    
    float Scale = max(WindowSize.width / SpriteSize.width,
                      WindowSize.height / SpriteSize.height);
    It->setScale(Scale);
    It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    It->setPosition(WindowSize/2.f);
    
    //It->setBlendFunc((BlendFunc){GL_ONE, GL_ZERO});
    It->setBlendFunc(BlendFunc::DISABLE);
    
    return It;
}

Node* MainDepot::createGameNode() const {
    Size WindowSize = windowSize();
    Size GameSize = gameSize();
    float Scale = min(WindowSize.width/GameSize.width, WindowSize.height/GameSize.height);
    
    Node* It = Node::create();
    It->setContentSize(GameSize);
    It->setScale(Scale);
    It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    It->setPosition(WindowSize/2.f);
    
    return It;
}
    
}  // namespace lettertracingcard
}  // namespace todoschool
