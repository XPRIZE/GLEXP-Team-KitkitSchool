//
//  TargetDummyNode.cpp on Aug 8, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TargetDummyNode.h"
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
    
    size_t bubbleCount() { return 5; }
}  // unnamed namespace


TargetDummyNode::TargetDummyNode()
    : TextLabel(nullptr)
{
}

bool TargetDummyNode::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

Size TargetDummyNode::defaultSize() {
    return contentSize();
}

void TargetDummyNode::clearInternals() {
    setContentSize(contentSize());
    
    TitleText.OnValueUpdate = [this](string&) {
        if (!TextLabel) { return; }
        TextLabel->setString(TitleText());
    };
}

void TargetDummyNode::refreshChildNodes() {
    removeAllChildren();
    Bubbles.clear();
    
    StarFallDepot Depot;
    Size CS = getContentSize();
    
    TextLabel = ([&] {
        Vec2 Translate = Vec2(0.f, Point(CS).length() * .10f);

        auto It = Label::createWithTTF(TitleText(), fontFace(), fontSize());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        It->setTextColor(Color4B(fontColor()));

        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(Depot.keyForBubblePop()));
        Actions.pushBack([&] {
            float Duration = Depot.keyForBubbleScatter() - Depot.keyForBubblePop();
            auto MB = EaseOut::create(MoveBy::create(Duration, Translate), 1.f);
            auto FO = FadeOut::create(Duration);
            return Spawn::create(MB, FO, nullptr);
        }());

        It->runAction(Sequence::create(Actions));
        addChild(It);
        return It;
    }());
    
    if (true) {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        It->setCascadeOpacityEnabled(true);
        It->setOpacity(255 * 1.f);
        
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(Depot.keyForBubblePop()));
        Actions.pushBack(FadeTo::create(0.f, 255 * 0.f));

        It->runAction(Sequence::create(Actions));
        addChild(It);
    }

    for (size_t BubbleID = 0; BubbleID < bubbleCount(); ++BubbleID) {
        float Radius = Point(CS).length() * random(.3f, 1.2f);
        float Theta = random(0.f, (float)(2.f * M_PI));
        Point Translate = Point(cos(Theta), sin(Theta)) * Radius;

        float ScaleStart = random(.05f, .2f);
        float ScaleStop = ScaleStart * 2;

        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        It->setScale(ScaleStart);
        It->setCascadeOpacityEnabled(true);
        It->setOpacity(255 * 0.f);
        
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(DelayTime::create(Depot.keyForBubblePop()));
        Actions.pushBack(FadeTo::create(0.f, 255 * 1.f));
        Actions.pushBack([&] {
            float Duration = Depot.keyForBubbleScatter() - Depot.keyForBubblePop();
            auto MB = EaseOut::create(MoveBy::create(Duration, Translate), 1.f);
            auto ST = ScaleTo::create(Duration, ScaleStop);
            auto FO = FadeOut::create(Duration);
            return Spawn::create(MB, ST, FO, nullptr);
        }());
        Actions.pushBack(CallFunc::create([this] {
            if (OnActionDidFinish)
                OnActionDidFinish(*this);
        }));

        It->runAction(Sequence::create(Actions));
        Bubbles.push_back(It);
        addChild(It);
    }
}

END_NS_STARFALL