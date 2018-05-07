//
//  MainDepot.cpp on Jul 15, 2016
//  KitkitSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"
#include <Managers/LanguageManager.hpp>
#include "Common/Basic/DeviceSpec.h"
#include "Common/Sounds/CommonSound.hpp"
//#include <Games/NumberTrace/Common/Repr/AllRepr.h>

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


namespace todoschool {
namespace comptrace {

std::string MainDepot::assetPrefix() const {
    return "ComprehensionTest/CompTrace";
}

std::string MainDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Sprite* MainDepot::createBackgroundSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/wordtracing_blackboard.jpg");
    
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


void MainDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect MainDepot::soundForWord(const std::string& Word) const {
    return CommonSound().soundForWord(Word);
}
    
SoundEffect MainDepot::soundForGoodAssetTouch() const {
    string P = "Common/Sounds/Effect";
    return SoundEffect(P + "/Cards_2.m4a");
}
    
SoundEffect MainDepot::soundForWorkComplete() const {
    string P = "Common/Sounds/Effect";
    return SoundEffect(P + "/Cards_4.m4a");
}



    
std::vector<SoundEffect> MainDepot::soundEffectsToPreload() const {
    vector<SoundEffect> Sounds;
    
    auto Push = [&](const SoundEffect& S) {
        // NB(xenosoz, 2016): Append it only if it's not empty.
        if (!S.empty())
            Sounds.push_back(S);
    };
    
    Push(soundForGoodAssetTouch());
    
    return Sounds;
}

}  // namespace comptrace
}  // namespace todoschool
