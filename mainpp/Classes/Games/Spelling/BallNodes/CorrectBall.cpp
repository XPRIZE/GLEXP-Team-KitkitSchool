//
//  CorrectBall.cpp -- A chameleon ball for the correct answer
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CorrectBall.h"
#include "../Utils/MainDepot.h"
#include <Games/WoodenPuzzles/Components/TargetTouchBody.h>


BEGIN_NS_SPELLING

using todoschool::woodenpuzzles::TargetTouchBody;


namespace {
    Size contentSize(BallSize BS) {
        switch (BS) {
            case BallSize::Small:
                return Size(180.f, 180.f);
            case BallSize::Large:
                return Size(258.f, 258.f);
        }
        CCLOGERROR("Unsupported BallSize in %s", __PRETTY_FUNCTION__);
    }
}  // unnamed namespace


CorrectBall::CorrectBall()
: YellowBall(nullptr)
, BlueBall(nullptr)
{
}

bool CorrectBall::init(BallSize BS) {
    if (!Super::init()) { return false; }
    
    TheBallSize = BS;

    clearInternals();
    refreshComponents();
    refreshChildNodes();
    
    return true;
}

Size CorrectBall::defaultSize(BallSize BS) {
    return contentSize(BS);
}


void CorrectBall::clearInternals() {
    setContentSize(contentSize(TheBallSize));
    setCascadeOpacityEnabled(true);
    
    Text.OnValueUpdate = [this](string&) {
        SoundForLetterName = MainDepot().soundForLetterName(Text());
    };
}

void CorrectBall::refreshComponents() {
    removeAllComponents();
    
    [&] {
        auto It = TargetTouchBody::create();
        
        It->OnTouchDidBegin.emplace_back([this](Touch*, Event*) -> bool {
            SoundForLetterName.play();
            return true;
        });
        
        addComponent(It);
    }();
}

void CorrectBall::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();
    
    YellowBall = ([&] {
        auto It = DummyBall::create(TheBallSize, BallKind::CorrectAnswer);
        It->Text.follow(Text);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
        return It;
    }());

    BlueBall = ([&] {
        auto It = DummyBall::create(TheBallSize, BallKind::ActiveAnswer);
        It->Text.follow(Text);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        It->setVisible(false);
        addChild(It);
        return It;
    }());
}

END_NS_SPELLING
