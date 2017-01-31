//
//  TargetTextNode.cpp on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TargetTextNode.h"
#include "../Utils/StarFallDepot.h"


BEGIN_NS_STARFALL

namespace {
    string contentSkin(int N = -1) {
        N = (N < 0 ? random(1, 5) : ((N + 4) % 5) + 1);

        return (StarFallDepot().assetPrefix() +
                format("/TargetTextNode/bubble_%d.png", N));
    }
    Size contentSize() { return Size(309.f, 308.f); }

    string fontFace() { return StarFallDepot().defaultFontFace(); }
    float fontSize() { return 100.f; }
    Color3B fontColor() { return Color3B(255, 255, 255); }

}  // unnamed namespace


TargetTextNode::TargetTextNode()
    : TextLabel(nullptr)
{
}

bool TargetTextNode::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    return true;
}

Size TargetTextNode::defaultSize() {
    return contentSize();
}

void TargetTextNode::onEnter() {
    Super::onEnter();
    scheduleUpdate();
}

void TargetTextNode::onExit() {
    Super::onExit();
    unscheduleUpdate();
}
    
void TargetTextNode::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);
    if (!GamePlaying()) { return; }

    Clock.update(Clock() + DeltaSeconds);
}

void TargetTextNode::clearInternals() {
    setContentSize(contentSize());

    // NB(xenosoz, 2016): Set default values.
    TitleText.update("");
    StartPosition.update(Point(500.f, 500.f));  // XXX
    StopPosition.update(Point(0.f, 0.f));
    Duration.update(1.f);
    Clock.update(0.f);

    
    // NB(xenosoz, 2016): Set callback handlers.

    TitleText.OnValueUpdate = [this](std::string&) {
        if (TextLabel)
            TextLabel->setString(TitleText());
        checkForTextMatch();
    };

    ActiveText.OnValueUpdate = [this](std::string&) {
        checkForTextMatch();
    };
    
    Duration.OnValueUpdate = [this](float&) {
        checkForTimeOver();
    };

    Clock.OnValueUpdate = [this](float&) {
        Size CS = getContentSize();

        float Alpha = min(max(0.f, Clock()), Duration()) / Duration();
        Point BaseP = StartPosition() * (1.f - Alpha) + StopPosition() * Alpha;
        float Phase = StartPhase() * (1.f - Alpha) + StopPhase() * Alpha;
        
        Point P = BaseP + (Point(cos(Phase), 0.f) *
                           Point(CS).length() *
                           SwingRatio());

        setPosition(P);

        checkForTimeOver();
    };
}

void TargetTextNode::refreshChildNodes() {
    removeAllChildren();

    Size CS = getContentSize();

    if (true) {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
    }
    
    TextLabel = ([&] {
        Label* It = Label::createWithTTF(TitleText(), fontFace(), fontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        It->setTextColor(Color4B(fontColor()));
        addChild(It);
        return It;
    }());
}

void TargetTextNode::checkForTextMatch() {
    if (TitleText() == ActiveText()) {
        auto Guard = ScopeGuard([this] { retain(); },
                                [this] { release(); });
        if (OnMatched)
            OnMatched(*this);
    }
}
    
void TargetTextNode::checkForTimeOver() {
    if (Clock() > Duration()) {
        auto Guard = ScopeGuard([this] { retain(); },
                                [this] { release(); });
        if (OnTimeOver)
            OnTimeOver(*this);
    }
}

END_NS_STARFALL
