//
//  MainDepot.cpp on Jul 15, 2016
//  enumaXprize
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"
#include "../../LetterTrace/Utils/MainDepot.h"
#include <Managers/LanguageManager.hpp>
#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Games/NumberTrace/Common/Repr/AllRepr.h>

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;


namespace todoschool {
namespace wordtrace {

std::string MainDepot::assetPrefix() const {
    return "WordTrace";
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
    Sprite* It = Sprite::create(assetPrefix() + "/wordtracing_blackboard.png");
    
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

BigLabel* MainDepot::createBonusTextLabel(const std::string& Text) const {
    const float FontSize = 500.f;
    BigLabel* It = BigLabel::createWithTTF(Text, defaultFont(), FontSize);

    It->setColor(Color3B(179, 230, 79));
    It->setOpacity(255 * .90f);

    return It;
}

cocos2d::Label* MainDepot::createNextLabel() const {
    Label* It = Label::createWithTTF("next", defaultFont(), 300.f);
    It->setColor(Color3B(255, 249, 237));
    It->setOpacity(GLubyte(255 * .90f));
    
    return It;
}

Button* MainDepot::createNextButton(const std::string& TeaserFileName) const {
    Node* RootNode = ([&] {
        Sprite* Bottom = createNextButtonBottomSprite();
        Sprite* Top = createNextButtonTopSprite();
        //Sprite* Effect = createNextButtonEffect();
        
        Size S = Bottom->getContentSize();
        Sprite* Teaser = createNextButtonTeaserSprite(TeaserFileName);
        
        Node* It = Node::create();
        
        It->setContentSize(S);
        int ZOrder = 0;
        
        Bottom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Bottom->setPosition(S/2.f);
        It->addChild(Bottom, ZOrder++);
        
        Teaser->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Teaser->setPosition(S/2.f);
        It->addChild(Teaser, ZOrder++);
        
        Top->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Top->setPosition(S/2.f);
        It->addChild(Top, ZOrder++);
        
        //Effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        //Effect->setPosition(S - Size(41, 48));
        //It->addChild(Effect, ZOrder++);
        
        return It;
    }());
    
    string EmptyImage = assetPrefix() + "/wordtracing_button_next_transparent.png";
    Button* B = Button::create(EmptyImage);
    B->addChild(RootNode);
    
    return B;
}

cocos2d::Label* MainDepot::createNextButtonLabel(const std::string& Text) const {
    Label* It = Label::createWithTTF(Text, defaultFont(), 140.f);
    It->setColor(Color3B(255, 249, 237));
    It->setOpacity(GLubyte(255 * .90f));

    return It;
}

void MainDepot::preloadSoundEffects() const {
    for (auto& Effect : soundEffectsToPreload()) {
        Effect.preload();
    }
}

SoundEffect MainDepot::soundForWord(const std::string& Word) const {
    return lettertrace::MainDepot().soundForWord(Word);
}
    
SoundEffect MainDepot::soundForGoodAssetTouch() const {
    return lettertrace::MainDepot().soundForGoodAssetTouch();
}
    
SoundEffect MainDepot::soundForWorkComplete() const {
    return lettertrace::MainDepot().soundForWorkComplete();
}

std::string MainDepot::videoFilePathByFileName(const std::string& FileName) const {
    return lettertrace::MainDepot().videoFilePathByFileName(FileName);
}

Sprite* MainDepot::createNextButtonBottomSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/wordtracing_button_next_shadow.png");
    return It;
}

Sprite* MainDepot::createNextButtonTopSprite() const {
    Sprite* It = Sprite::create(assetPrefix() + "/tracing_image_tapes.png");
    return It;
}

Sprite* MainDepot::createNextButtonEffect() const {
    Sprite* It = Sprite::create(assetPrefix() + "/wordtracing_button_next_effect.png");
    return It;
}

Sprite* MainDepot::createNextButtonTeaserSprite(std::string TeaserFileName) const {
    // NB(xenosoz, 2016): I baked all sprites for necessary videos.
    
    string CountryCode = LanguageManager::getInstance()->getCurrentLanguageCode();
    string Prefix = assetPrefix() + "/Teasers." + CountryCode;
    Sprite* It = Sprite::create(Prefix + "/" + TeaserFileName);
    
    if (!It) {
        CCLOGERROR("Sprite for (%s) not ready in MainDepot::createNextButtonTeaserSprite", TeaserFileName.c_str());
        
        // NB(xenosoz, 2016): Prepare a question mark instead.
        return Sprite::create(Prefix + "/%3f.png");
    }
    
    return It;
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

}  // namespace wordtrace
}  // namespace todoschool
