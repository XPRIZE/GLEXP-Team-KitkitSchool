//
//  AnswerBall.cpp -- Blue answer ball with flexible size
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "AnswerBall.h"
#include "../Utils/SpellingMainDepot.h"
#include "Common/Components/TargetDragBody.h"


BEGIN_NS_SPELLING



namespace {
    Size activeContentSize(BallSize BS) {
        switch (BS) {
            case BallSize::Small:
                return Size(228.f, 228.f);
            case BallSize::Large:
                return Size(260.f, 260.f);
        }
    }

    Size passiveContentSize(BallSize BS) {
        switch (BS) {
            case BallSize::Small:
                return Size(228.f, 228.f);
            case BallSize::Large:
                return Size(228.f, 228.f);
        }
    }

    Size contentSize(BallSize BS) {
        auto A = activeContentSize(BS);
        auto P = passiveContentSize(BS);

        return Size(max(A.width, P.width),
                    max(A.height, P.height));
    }
    
    int localZOrderForActiveBall() { return 10; }
    int localZOrderForPassiveBall() { return 0; }
}  // unnamed namespace


AnswerBall::AnswerBall()
: ActiveBall(nullptr)
, PassiveBall(nullptr)
{
}

bool AnswerBall::init(BallSize BS) {
    if (!Super::init()) { return false; }
    
    TheBallSize = BS;
    
    clearInternals();
    refreshComponents();
    refreshChildNodes();

    return true;
}

void AnswerBall::setPosition(float x, float y) {
    Super::setPosition(x, y);
    
    auto Parent = getParent();
    if (!Parent) { return; }
    
    Point LocalP(x, y);
    Point WorldP = Parent->convertToWorldSpace(LocalP);
    
    if (PositionInWorldSpace() != WorldP)
        PositionInWorldSpace.update(WorldP);
}

void AnswerBall::playHitSound() {
    SoundForHit.play();
}

Size AnswerBall::defaultSizeForActiveBall(BallSize BS) {
    return activeContentSize(BS);
}

Size AnswerBall::defaultSizeForPassiveBall(BallSize BS) {
    return passiveContentSize(BS);
}


void AnswerBall::clearInternals() {
    setContentSize(contentSize(TheBallSize));
    setCascadeOpacityEnabled(true);
    
    SoundForLetterName = SoundEffect::emptyEffect();
    SoundForHit = MainDepot().soundForCardHit();
    
    Text.OnValueUpdate = [this](string&) {
        refreshChildNodes();
        SoundForLetterName = MainDepot().soundForLetterName(Text());
    };
    
    Opacity.OnValueUpdate = [this](GLubyte&) {
        setOpacity(Opacity());
    };

    PositionInWorldSpace.OnValueUpdate = [this](Point&) {
        auto Parent = getParent();
        if (!Parent) { return; }
        
        Point P = Parent->convertToNodeSpace(PositionInWorldSpace());
        setPosition(P);
    };
}

void AnswerBall::refreshComponents() {
    removeAllComponents();

    [&] {
        auto It = TargetDragBody::create();
        
        It->OnDragDidBegin.emplace_back([this](Touch* T, Event* E) -> bool {
            makeActive();
            SoundForLetterName.play();
            
            NodeScopeGuard Guard(this);
            OnDragDidBegin(T, E);

            return true;
        });
        
        It->OnDrag.emplace_back([this](Touch* T, Event* E) {
            if (!OnDrag) { return; }

            NodeScopeGuard Guard(this);
            OnDrag(T, E);
        });

        It->OnDragDidEnd.emplace_back([this](Touch* T, Event* E) {
            makePassive();
            
            NodeScopeGuard Guard(this);
            OnDragDidEnd(T, E);
        });
        
        addComponent(It);
    }();
}

void AnswerBall::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();

    ActiveBall = ([&] {
        auto It = DummyBall::create(TheBallSize, BallKind::ActiveAnswer);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        It->Text.follow(Text);
        
        It->setOpacity(0);
        addChild(It, localZOrderForActiveBall());
        return It;
    }());
    
    PassiveBall = ([&] {
        auto It = DummyBall::create(TheBallSize, BallKind::PassiveAnswer);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        It->Text.follow(Text);

        addChild(It, localZOrderForPassiveBall());
        return It;
    }());
}

void AnswerBall::makeActive() {
    stopAllActions();
    
    if (ActiveBall)
        ActiveBall->setOpacity(255);
    
    setLocalZOrder(10);
}

void AnswerBall::makePassive() {
    stopAllActions();

    if (ActiveBall)
        ActiveBall->setOpacity(0);
    
    auto Parent = getParent();
    if (!Parent) { return; }
    
    Point P = Parent->convertToNodeSpace(HomePositionInWorldSpace());
    runAction(Sequence::create(MoveTo::create(.1f, P),
                               CallFunc::create([this] { setLocalZOrder(0); }),
                               nullptr));
}

END_NS_SPELLING
