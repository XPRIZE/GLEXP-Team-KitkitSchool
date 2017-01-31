//
//  MainDepot.cpp on Jun 30, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "MainDepot.h"
#include <Managers/LanguageManager.hpp>
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Common/Sounds/Pam_enUS.h>
#include <Common/Sounds/Imma_swTZ.h>

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


namespace todoschool {
namespace lettertrace {

std::string MainDepot::assetPrefix() const {
    return "LetterTrace";
}

Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Sprite* MainDepot::createBackgroundSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/lettertracing_image_bg.png");
    
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
 
Label* MainDepot::createNextLabel() const {
    Label* It = Label::createWithTTF("next", "fonts/TodoSchoolV2.ttf", 300.f);
    It->setColor(Color3B(105, 68, 31));

    return It;
}

Button* MainDepot::createNextButton(std::string T) const {
    Node* RootNode = ([&] {
        Sprite* Bottom = createNextButtonBottomSprite();
        Sprite* Top = createNextButtonTopSprite();
        Sprite* Effect = createNextButtonEffect();
        
        Size S = Bottom->getContentSize();
        Sprite* Label = createNextButtonTextSprite(T);

        Node* It = Node::create();
        
        It->setContentSize(S);
        int ZOrder = 0;
        
        Bottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Bottom->setPosition(S/2.f);
        It->addChild(Bottom, ZOrder++);
        
        Label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Label->setPosition(S/2.f);
        It->addChild(Label, ZOrder++);
        
        Top->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Top->setPosition(S/2.f);
        It->addChild(Top, ZOrder++);
        
        Effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Effect->setPosition(S - Size(41, 48));
        It->addChild(Effect, ZOrder++);
        
        return It;
    }());
    
    string EmptyImage = assetPrefix() + "/lettertracing_button_next_layer-transparent.png";
    Button* B = Button::create(EmptyImage);
    B->addChild(RootNode);

    return B;
}
    
void MainDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect MainDepot::soundForLetter(const string& Letter) const {
    switch (LanguageManager::getInstance()->getCurrentLanguage()) {
        default:
            CCLOGERROR("No proper language is found in %s", __PRETTY_FUNCTION__);
            /* Fall through */
        case LanguageManager::ENGLISH: return Pam_enUS().soundForLetterName(Letter);
        case LanguageManager::SWAHILI: return Imma_swTZ().soundForLetterName(Letter);
    }
}

SoundEffect MainDepot::soundForWord(const string& Word) const {
    switch (LanguageManager::getInstance()->getCurrentLanguage()) {
        default:
            CCLOGERROR("No proper language is found in %s", __PRETTY_FUNCTION__);
            /* Fall through */
        case LanguageManager::ENGLISH: return Pam_enUS().soundForWord(Word);
        case LanguageManager::SWAHILI: return Imma_swTZ().soundForWord(Word);
    }
}

SoundEffect MainDepot::soundForLetterBetweenWords() const {
    if (LanguageManager::getInstance()->isEnglish()) {
        string P = assetPrefix() + "/TempSounds";
        return SoundEffect(P + "/Is_For.m4a");
    }

    if (LanguageManager::getInstance()->isSwahili()) {
        // NB(xenosoz, 2016): We do "A, Asali" for Swahili. No sound between words.
        return SoundEffect::emptyEffect();
    }

    // NB(xenosoz, 2016): Fallback to English sound.
    string P = assetPrefix() + "/TempSounds";
    return SoundEffect(P + "/Is_For.m4a");
}

SoundEffect MainDepot::soundForTraceEnd() const {
    string P = assetPrefix() + "/TempSounds";
    return SoundEffect(P + "/Cards_1.m4a");
}

SoundEffect MainDepot::soundForGoodAssetTouch() const {
    string P = assetPrefix() + "/TempSounds";
    return SoundEffect(P + "/Cards_2.m4a");
}

SoundEffect MainDepot::soundForBadAssetTouch() const {
    string P = assetPrefix() + "/TempSounds";
    return SoundEffect(P + "/Cards_3.m4a");
}

SoundEffect MainDepot::soundForWorkComplete() const {
    string P = assetPrefix() + "/TempSounds";
    return SoundEffect(P + "/Cards_4.m4a");
}

string MainDepot::videoFilePathByFileName(string FileName) {
    string It;
    string CountryCode = LanguageManager::getInstance()->getCurrentLanguageCode();

    It = assetPrefix() + "/BonusVideos." + CountryCode + "/" + FileName;
    if (FileUtils::getInstance()->isFileExist(It))
        return It;

    It = assetPrefix() + "/BonusVideos/" + FileName;
    if (FileUtils::getInstance()->isFileExist(It))
        return It;

    return "";
}

Sprite* MainDepot::createNextButtonBottomSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/lettertracing_button_next_layer-01.png");
    return It;
}

Sprite* MainDepot::createNextButtonTopSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/lettertracing_button_next_layer-02.png");
    return It;
}
    
Sprite* MainDepot::createNextButtonEffect() const {
    Sprite* It = Sprite::create(assetPrefix() + "/lettertracing_button_next_effect.png");
    return It;
}

Sprite* MainDepot::createNextButtonTextSprite(const std::string& T) const {
    // NB(xenosoz, 2016): Label::createWithTTF(T, ..., 460.f) was unstable,
    //   I baked all sprites for necessary letters.

    string CountryCode = LanguageManager::getInstance()->getCurrentLanguageCode();
    string Prefix = assetPrefix() + "/Teasers." + CountryCode;

    // NB(xenosoz, 2016): 'A' == 'a' happens in case-insensitive filesystem.
    //   But we may always assume '%41' != '%61'.

    string EscapedText = [&] {
        stringstream SS;
        for (auto C : T)
            SS << StringUtils::format("%%%x", C);
        
        return SS.str();
    }();

    Sprite* It = Sprite::create(Prefix + "/" + EscapedText + ".png");

    if (!It) {
        CCLOGERROR("Sprite for text (%s) not ready in MainDepot::createNextButtonTextSprite", T.c_str());

        // NB(xenosoz, 2016): Prepare a question mark instead.
        return Sprite::create(Prefix + "/%3f.png");
    }

    return It;
}

vector<SoundEffect> MainDepot::soundEffectsToPreload() const {
    vector<SoundEffect> Sounds;
    
    auto Push = [&](const SoundEffect& S) {
        // NB(xenosoz, 2016): Append it only if it's not empty.
        if (!S.empty())
            Sounds.push_back(S);
    };
    
    for (char C = 'a'; C <= 'z'; ++C)
        Push(soundForLetter(string(1, C)));
    
    Push(soundForLetterBetweenWords());
    Push(soundForTraceEnd());
    Push(soundForGoodAssetTouch());
    Push(soundForBadAssetTouch());
    Push(soundForWorkComplete());
    
    return Sounds;
}

}  // namespace lettertrace
}  // namespace todoschool
