//
//  MainDepot.cpp on Apr 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "MainDepot.h"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>


BEGIN_NS_NUMBERTRACE

string MainDepot::assetPrefix() const {
    return "NumberTrace";
}
    
Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Sprite* MainDepot::createBackgroundSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/Trace_Background.png");
    
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

SoundEffect MainDepot::soundForNumber(int Number) const {
    if (LanguageManager::getInstance()->isEnglish()) {
        return englishSoundForNumber(Number);
    }
    
    if (LanguageManager::getInstance()->isSwahili()) {
        return swahiliSoundForNumber(Number);
    }

    // NB(xenosoz, 2016): Fallback to english sound.
    return englishSoundForNumber(Number);
}

SoundEffect MainDepot::soundForTraceEnd() const {
    string P = "Common/Sounds/Effect";
    return SoundEffect(P + "/Cards_1.m4a");
}
    
SoundEffect MainDepot::soundForGoodAssetTouch() const {
    string P = "Common/Sounds/Effect";
    return SoundEffect(P + "/Cards_2.m4a");
}

SoundEffect MainDepot::soundForBadAssetTouch() const {
    string P = "Common/Sounds/Effect";
    return SoundEffect(P + "/Cards_3.m4a");
}

SoundEffect MainDepot::soundForWorkComplete() const {
    string P = "Common/Sounds/Effect";
    return SoundEffect(P + "/Cards_4.m4a");
}

Problem::AssetType MainDepot::assetTypeForString(const string &S) {
    if (S == "Ant") { return Problem::AssetType::Ant; }
    if (S == "Bee") { return Problem::AssetType::Bee; }
    if (S == "Beetle") { return Problem::AssetType::Beetle; }
    if (S == "BlueButterfly") { return Problem::AssetType::BlueButterfly; }
    if (S == "Cockroach") { return Problem::AssetType::Cockroach; }
    if (S == "Ladybug") { return Problem::AssetType::Ladybug; }
    if (S == "Moth") { return Problem::AssetType::Moth; }
    if (S == "Spider") { return Problem::AssetType::Spider; }
    if (S == "StagBeetle") { return Problem::AssetType::StagBeetle; }
    if (S == "YellowButterfly") { return Problem::AssetType::YellowButterfly; }
    
    // XXX: Fallback to ant
    CCLOGERROR("Unsupported asset type string(%s) in %s", S.c_str(), __PRETTY_FUNCTION__);
    return Problem::AssetType::Ant;
}

string MainDepot::stringForAssetType(Problem::AssetType T) {
    switch (T) {
        case Problem::AssetType::Ant: return "Ant";
        case Problem::AssetType::Bee: return "Bee";
        case Problem::AssetType::Beetle: return "Beetle";
        case Problem::AssetType::BlueButterfly: return "BlueButterfly";
        case Problem::AssetType::Cockroach: return "Cockroach";
        case Problem::AssetType::Ladybug: return "Ladybug";
        case Problem::AssetType::Moth: return "Moth";
        case Problem::AssetType::Spider: return "Spider";
        case Problem::AssetType::StagBeetle: return "StagBeetle";
        case Problem::AssetType::YellowButterfly: return "YellowButterfly";
    }
    return "";
}

vector<SoundEffect> MainDepot::soundEffectsToPreload() const {
    vector<SoundEffect> Sounds;
    
    auto Push = [&](const SoundEffect& S) {
        // NB(xenosoz, 2016): Append it only if it's not empty.
        if (!S.empty())
            Sounds.push_back(S);
    };
    
    for (int I = 0; I <= 100; ++I)
        Push(soundForNumber(I));
    
    Push(soundForTraceEnd());
    Push(soundForGoodAssetTouch());
    Push(soundForBadAssetTouch());
    Push(soundForWorkComplete());
    
    return Sounds;
}

SoundEffect MainDepot::englishSoundForNumber(int Number) const {
    string P = assetPrefix() + "/Sounds";
    
    if (1 <= Number && Number <= 10) {
        return SoundEffect(P + StringUtils::format("/num_%02d.wav", Number));
    }
    
    return SoundEffect();
}

SoundEffect MainDepot::swahiliSoundForNumber(int Number) const {
    string P = assetPrefix() + "/Sounds";
    
    if (1 <= Number && Number <= 30) {
        return SoundEffect(P + StringUtils::format("/snumber-%02d.wav", Number));
    }
    
    return SoundEffect();
}

END_NS_NUMBERTRACE
