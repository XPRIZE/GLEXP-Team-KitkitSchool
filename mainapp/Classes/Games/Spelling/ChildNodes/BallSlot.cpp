//
//  BallSlot.h -- Empty/full ball slot for GameBoard
//  TodoSchool on Sep 14, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "BallSlot.h"
#include "../Utils/SpellingMainDepot.h"


BEGIN_NS_SPELLING

namespace {
    string contentSkin(BallSize BS) {
        string P = MainDepot().assetPrefix();

        switch (BS) {
            case BallSize::Small:
                return P + "/GameBoard/spelling_frame_hole_xsmall.png";
            case BallSize::Large:
                return P + "/GameBoard/spelling_frame_hole.png";
        }
        CCLOGERROR("Unsupported contentSkin type in %s", __PRETTY_FUNCTION__);
    }

    Size contentSize(BallSize BS) {
        switch (BS) {
            case BallSize::Small: return Size(192.f, 192.f);
            case BallSize::Large: return Size(278.f, 278.f);
        }
        CCLOGERROR("Unsupported contentSkin type in %s", __PRETTY_FUNCTION__);
    }
}  // unnamed namespace


BallSlot::BallSlot()
: TheBall(nullptr)
{
}

bool BallSlot::init(BallSize BS) {
    if (!Super::init()) { return false; }
    
    TheBallSize = BS;
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

bool BallSlot::empty() {
    if (!TheBall) { return true; }
    
    return !TheBall->isVisible();
}

void BallSlot::ejectBall() {
    if (!TheBall) { return; }
    Vector<FiniteTimeAction*> Actions;

    Actions.pushBack([&] {
        Vector<FiniteTimeAction*> It;
        
        if (MainDepot().allowFadeInOutForAnswerBall()) {
            Point MoveP = TheBall->getPosition();
            MoveP += Vec2(random(-10.f, 10.f), random(10.f, -100.f) - 120.f);
            It.pushBack(EaseOut::create(MoveTo::create(0.8f, MoveP), 2.f));
            It.pushBack(FadeOut::create(0.8f));
        }
        else {
            Point MoveP = TheBall->getPosition();
            MoveP.y = 0.f - MainDepot().gameSize().height;
            It.pushBack(EaseIn::create(MoveTo::create(0.8f, MoveP), 2.f));
        }
        return Spawn::create(It);
    }());

    Actions.pushBack(CallFunc::create([this] {
        TheBall->setVisible(false);
        TheBall->setOpacity(255);
    }));
    
    TheBall->runAction(Sequence::create(Actions));
}

void BallSlot::fillSlot() {
    if (!TheBall || !empty()) { return; }
    
    Size CS = getContentSize();
    TheBall->setPosition(CS / 2.f);
    TheBall->setVisible(true);

    if (OnSlotDidFill) {
        NodeScopeGuard Guard(this);
        OnSlotDidFill();
    }
}

Size BallSlot::defaultSize(BallSize BS) {
    return contentSize(BS);
}


void BallSlot::clearInternals() {
    setContentSize(contentSize(TheBallSize));
    setCascadeOpacityEnabled(true);
    
    BallText.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void BallSlot::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();

    [&] {
        auto It = Sprite::create(contentSkin(TheBallSize));
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        addChild(It);
        return It;
    }();
    
    TheBall = ([&] {
        auto It = CorrectBall::create(TheBallSize);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        It->Text.follow(BallText);
        
        addChild(It);
        return It;
    }());
}

END_NS_SPELLING
