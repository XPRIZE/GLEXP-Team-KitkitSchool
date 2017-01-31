//
//  WordImage.cpp -- The image part of the problem
//  TodoSchool on Sep 16, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WordImage.h"
#include "../Utils/MainDepot.h"
#include <Games/WoodenPuzzles/Components/TargetTouchBody.h>


BEGIN_NS_TUTORIALTRACE

using todoschool::woodenpuzzles::TargetTouchBody;

namespace {
    Size contentSize() { return Size(1111.f, 820.f); }

    string workSkinCover() {
        return MainDepot().assetPrefix() + "/GameBoard/spelling_wordimage_edge_gradient.png";
    }
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

    TheProblem.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
    };
}

void WordImage::refreshComponents() {
    [&] {
        auto It = TargetTouchBody::create();

        It->OnTouchDidBegin.emplace_back([this](Touch*, Event*) -> bool {
            SoundForWord.play();
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

END_NS_TUTORIALTRACE
