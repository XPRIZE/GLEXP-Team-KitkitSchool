//
//  StarFallDepot.cpp on Jul 15, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "StarFallDepot.h"
#include <Managers/LanguageManager.hpp>
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Games/NumberTrace/Common/Repr/AllRepr.h>
#include <Common/Sounds/Pam_enUS.h>
#include <Common/Sounds/Imma_swTZ.h>

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


BEGIN_NS_STARFALL

std::string StarFallDepot::assetPrefix() const {
    return "StarFall";
}

std::string StarFallDepot::defaultFontFace() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size StarFallDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size StarFallDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Node* StarFallDepot::createBackgroundNode() const {
    Size WindowSize = windowSize();

    // NB(xenosoz, 2016): The background node.
    Node* BN = Node::create();
    BN->setContentSize(WindowSize);
    
    BN->addChild([&] {
        // NB(xenosoz, 2016): The starry night.
        Sprite* It = Sprite::create(assetPrefix() + "/Background/night_sky.png");
        Size SpriteSize = It->getContentSize();
        
        float Scale = max(WindowSize.width / SpriteSize.width,
                          WindowSize.height / SpriteSize.height);
        It->setScale(Scale);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        It->setPosition(Point(WindowSize.width/2.f, WindowSize.height));
        
        It->setBlendFunc(BlendFunc::DISABLE);
        return It;
    }());

    BN->addChild([&] {
        // NB(xenosoz, 2016): A leaf on the left.
        Sprite* It = Sprite::create(assetPrefix() + "/Background/leaf_left.png");
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(0.f, WindowSize.height));

        return It;
    }());
    
    BN->addChild([&] {
        // NB(xenosoz, 2016): A leaf on the right.
        Sprite* It = Sprite::create(assetPrefix() + "/Background/leaf_right.png");
        It->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        It->setPosition(WindowSize);
        
        return It;
    }());
    
    return BN;
}

Node* StarFallDepot::createGameNode() const {
    Size WindowSize = windowSize();
    Size GameSize = gameSize();
    float Scale = min(WindowSize.width/GameSize.width, WindowSize.height/GameSize.height);
    
    Node* It = Node::create();
    It->setContentSize(GameSize);
    It->setScale(Scale);
    It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    It->setPosition(Point(WindowSize.width/2.f, 0.f));
    
    return It;
}

Json::Value StarFallDepot::jsonForMainGameWork() {
	if (LanguageManager::getInstance()->isSwahili()) {
			return swahiliJsonForMainGameWork();
	}
    return englishJsonForMainGameWork();
}
   
SymbolLayoutType StarFallDepot::defaultSymbolLayout() const {
    return {
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", "'"},
        {"z", "x", "c", "v", "b", "n", "m"},
    };
}

SymbolSetType StarFallDepot::defaultEnabledSymbolSet() const {
    SymbolSetType S;
    
    for (const auto& Row : defaultSymbolLayout())
        for (const auto& Word : Row)
            S.insert(Word);
    
    return S;
}    

float StarFallDepot::keyForBubblePop() const {
    return .1f;
}

float StarFallDepot::keyForBubbleScatter() const {
    return .5f;
}

void StarFallDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect StarFallDepot::soundForKeyInput() const {
    return SoundEffect(assetPrefix() + "/Sounds/key_input.m4a");
}
    
SoundEffect StarFallDepot::soundForTextHit(int N /* = -1 */) const {
    if (N < 0)
        N = random(0, 5);

    return SoundEffect(assetPrefix() + StringUtils::format("/Sounds/text_hit.%d.m4a", N));
}
    
SoundEffect StarFallDepot::soundForTextMiss() const {
    return SoundEffect(assetPrefix() + "/Sounds/text_miss.m4a");
}
    
SoundEffect StarFallDepot::soundForTargetBirth() const {
    return SoundEffect(assetPrefix() + "/Sounds/target_birth.m4a");
}

SoundEffect StarFallDepot::soundForTargetDeath() const {
    return SoundEffect(assetPrefix() + "/Sounds/target_death.m4a");
}

SoundEffect StarFallDepot::soundForWord(const string& Word) const {
    if (LanguageManager::getInstance()->isSwahili()) {
        return Imma_swTZ().soundForWord(Word);
    }
    
    return Pam_enUS().soundForWord(Word);
}

Json::Value StarFallDepot::swahiliJsonForMainGameWork()
{
    string Filename = assetPrefix() + "/starfall.sw.json";
    string JsonStr = FileUtils::getInstance()->getStringFromFile(Filename);

    Json::Value JsonValue;
    Json::Reader JsonReader;
    if (!JsonReader.parse(JsonStr, JsonValue)) {
        CCLOGERROR("JSON reader parse error: %s", Filename.c_str());
        JsonValue.clear();
    }
    
    return JsonValue;
}

Json::Value StarFallDepot::englishJsonForMainGameWork()
{
    string Filename = assetPrefix() + "/starfall.en.json";
    string JsonStr = FileUtils::getInstance()->getStringFromFile(Filename);

    Json::Value JsonValue;
    Json::Reader JsonReader;
    if (!JsonReader.parse(JsonStr, JsonValue)) {
        CCLOGERROR("JSON reader parse error: %s", Filename.c_str());
        JsonValue.clear();
    }
    
    return JsonValue;
}


std::vector<SoundEffect> StarFallDepot::soundEffectsToPreload() const {
    return {
        soundForKeyInput(),
        soundForTextHit(0),
        soundForTextHit(1),
        soundForTextHit(2),
        soundForTextHit(3),
        soundForTextHit(4),
        soundForTextHit(5),
        soundForTextMiss(),
        soundForTargetBirth(),
        soundForTargetDeath()
    };
}

END_NS_STARFALL
