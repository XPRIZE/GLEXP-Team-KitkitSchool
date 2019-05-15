//
//  WordImage.cpp -- The image part of the problem
//  TodoSchool on Sep 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellingWordImage.h"
#include "../Utils/SpellingMainDepot.h"
#include "Common/Components/TargetTouchBody.h"


BEGIN_NS_SPELLING


namespace {
    Size contentSize() { return Size(1111.f, 820.f); }

    string workSkinCover() {
        return MainDepot().assetPrefix() + "/GameBoard/spelling_wordimage_edge_gradient.png";
    }
    
    bool preventNewSoundEffectWhilePlaying() { return true; }
    int actionTagForSound() { return 100; }
}


bool WordImage::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshComponents();
    refreshChildNodes();
    
    return true;
}

void WordImage::clearInternals() {
    setContentSize(contentSize());
    setCascadeOpacityEnabled(true);
    SoundForWord = SoundEffect::emptyEffect();
    IsSoundPlaying = false;

    TheProblem.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
        SoundForWord = MainDepot().soundForWord(TheProblem().Word);
    };
}

void WordImage::refreshComponents() {
    [&] {
        auto It = TargetTouchBody::create();

        It->OnTouchDidBegin.emplace_back([this](Touch*, Event*) -> bool {
            playWordSound();
            return true;
        });

        addComponent(It);
    }();
}

void WordImage::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();

    [&] {
        // NB(xenosoz, 2016): The word image
        auto WorkSkin = MainDepot().assetPrefix() + "/WordImages/" + TheProblem().ImageFilename;
        auto It = Sprite::create(WorkSkin);
        if (!It) { return (It = nullptr); };
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        addChild(It);
        return It;
    }();

    [&] {
        // NB(xenosoz, 2016): Word image gradient
        auto It = Sprite::create(workSkinCover());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        addChild(It);
        return It;
    }();
}

void WordImage::playWordSound() {
    if (preventNewSoundEffectWhilePlaying() && IsSoundPlaying) { return; }

    SoundForWord.stop();
    SoundForWord.play();
    IsSoundPlaying = true;

    auto A = Sequence::create(DelayTime::create(TheProblem().SoundDuration),
                              CallFunc::create([this] { IsSoundPlaying = false; }),
                              nullptr);
    A->setTag(actionTagForSound());

    stopActionByTag(actionTagForSound());
    runAction(A);
}

END_NS_SPELLING
