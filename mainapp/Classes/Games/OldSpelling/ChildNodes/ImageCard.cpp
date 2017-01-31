//
//  ImageCard.cpp -- Work image card
//  TodoSchool on Aug 24, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ImageCard.h"
#include "../Utils/SpellingDepot.h"


BEGIN_NS_SPELLING;

namespace {
    Size contentSize() { return Size(692.f, 550.f); }

    string contentSkin(const Work& W) {
        return (SpellingDepot().assetPrefix() + "/WordCards" +
                format("/%s.%s.png", W.WorkID.c_str(), W.Word.c_str()));
    }
}  // unnamed namespace


bool ImageCard::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void ImageCard::clearInternals() {
    setContentSize(contentSize());

    TheWork.OnValueUpdate = [this](Work&) {
        refreshChildNodes();
    };
}

void ImageCard::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();
    
    [&] {
        auto It = Sprite::create(contentSkin(TheWork()));
        if (!It) { return (It = nullptr); }

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
        return It;
    }();
}

END_NS_SPELLING;
