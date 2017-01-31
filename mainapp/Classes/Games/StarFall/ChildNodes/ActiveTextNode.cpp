//
//  ActiveTextNode.cpp on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "ActiveTextNode.h"
#include "../Utils/StarFallDepot.h"


BEGIN_NS_STARFALL

namespace {
    string contentSkin() {
        return StarFallDepot().assetPrefix() + "/ActiveTextNode/wood_screen.png";
    }
    Size contentSize() { return Size(735.f, 185.f); }
    Rect textRegion() { return Rect(140.f, 11.f, 453.f, 146.f); }

    string fontFace() { return StarFallDepot().defaultFontFace(); }
    float fontSize() { return 100.f; }

    Color3B regularColor() { return Color3B(74, 70, 67); }
    Color3B disabledColor() { return Color3B(148, 100, 63); }
}  // unnamed namespace


ActiveTextNode::ActiveTextNode()
    : ActiveTextLabel(nullptr)
    , BadTextLabel(nullptr)
{
}

bool ActiveTextNode::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    return true;
}

void ActiveTextNode::clearInternals() {
    setContentSize(contentSize());
    
    // NB(xenosoz, 2016): Set default values.
    ActiveText.update("");
    BadText.update("");

    ActiveText.OnValueUpdate = [this](std::string&) {
        if (ActiveTextLabel)
            ActiveTextLabel->setString(ActiveText());

        if (!ActiveText().empty()) {
            hideBadText();
            showActiveText();
        }
    };

    BadText.OnValueUpdate = [this](std::string&) {
        if (BadTextLabel)
            BadTextLabel->setString(BadText());
        
        if (!BadText().empty()) {
            hideActiveText();
            showBadText();
        }
    };
}

void ActiveTextNode::refreshChildNodes() {
    Size CS = getContentSize();

    removeAllChildren();
    
    if (true) {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
    }
    
    ActiveTextLabel = ([&] {
        Label* It = Label::createWithTTF(ActiveText(), fontFace(), fontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(textRegion().getMidX(),
                              textRegion().getMidY()));

        It->setTextColor(Color4B(regularColor()));
        addChild(It);
        return It;
    }());
    
    BadTextLabel = ([&] {
        Label* It = Label::createWithTTF(BadText(), fontFace(), fontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(textRegion().getMidX(),
                              textRegion().getMidY()));

        It->setTextColor(Color4B(disabledColor()));
        It->setVisible(false);
        addChild(It);
        return It;
    }());
}
    
void ActiveTextNode::showActiveText() {
    if (!ActiveTextLabel) { return; }
    ActiveTextLabel->setVisible(true);
}

void ActiveTextNode::showBadText() {
    if (!BadTextLabel) { return; }
    BadTextLabel->setVisible(true);
    
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For node hiding timing
        hideBadText();
    }, 1.f, "ActiveTextNode::hideBadText");
}

void ActiveTextNode::hideActiveText() {
    if (!ActiveTextLabel) { return; }
    ActiveTextLabel->setVisible(false);
}

void ActiveTextNode::hideBadText() {
    unschedule("ActiveTextNode::hideBadText");

    if (!BadTextLabel) { return; }
    BadTextLabel->setVisible(false);
}

END_NS_STARFALL
