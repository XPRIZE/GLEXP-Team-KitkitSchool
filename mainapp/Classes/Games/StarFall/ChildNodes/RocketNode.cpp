//
//  RocketNode.cpp on Jul 26, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "RocketNode.h"
#include "../Utils/StarFallDepot.h"


BEGIN_NS_STARFALL

namespace {
    string contentSkin() {
        return StarFallDepot().assetPrefix() + "/RocketNode/firefly.png";
    }
    Size contentSize() {
        return Size(209.f, 159.f);
    }
}  // unnamed namespace


RocketNode::RocketNode()
{
}

bool RocketNode::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void RocketNode::clearInternals() {
    setContentSize(contentSize());
    setCascadeOpacityEnabled(true);
    setOpacity(255 * 0.00f);
    
    BasePosition.OnValueUpdate = [this](Point&) {
        float EaseRate = 1.5f;

        setOpacity(255 * 0.00f);
        setPosition(BasePosition());

        auto Appear = ([&] {
            auto FO = FadeTo::create(0.f, 255 * 0.00f);
            auto FI = EaseOut::create(FadeTo::create(1.f, 255 * 1.00f), EaseRate);
            return Sequence::create(FO, FI, nullptr);
        }());

        auto Breathe = ([&] {
            float LargeScale = 1.1f;
            float SmallScale = 0.9f;

            auto Stress = EaseOut::create(ScaleTo::create(.8f, LargeScale), EaseRate);
            auto Relax = EaseIn::create(ScaleTo::create(1.2f, SmallScale), EaseRate);
            return Repeat::create(Sequence::create(Stress, Relax, nullptr), 50);
        }());
        
        stopAllActions();
        runAction(Spawn::create(Appear, Breathe, nullptr));
    };

    TargetPosition.OnValueUpdate = [this](Point&) {
        StarFallDepot Depot;
        
        Point CP = getPosition();
        Point TP = TargetPosition();
        float Distance = (TP - CP).length();
        Vec2 UnitDir = (TP - CP).getNormalized();
        Vec2 PerpDir = UnitDir.getPerp();

        ccBezierConfig BC = ([&] {
            float Diversity = Distance * .5f;
            Point A = CP + (UnitDir * Distance / 3.f) + (PerpDir * random(-Diversity, Diversity));
            Point B = TP - (UnitDir * Distance / 3.f) + (PerpDir * random(-Diversity, Diversity));

            ccBezierConfig It;
            It.endPosition = TP;
            It.controlPoint_1 = A;
            It.controlPoint_2 = B;
            return It;
        }());

        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack([&] {
            auto BT = BezierTo::create(Depot.keyForBubblePop(), BC);
            auto ST = ScaleTo::create(Depot.keyForBubblePop(), 1.f);
            auto FI = FadeIn::create(Depot.keyForBubblePop());
            return Spawn::create(BT, ST, FI, nullptr);
        }());
        Actions.pushBack(CallFunc::create([this] {
            auto Guard = ScopeGuard([this] { retain(); },
                                    [this] { release(); });
            if (OnActionDidFinish)
                OnActionDidFinish(*this);
        }));

        stopAllActions();
        runAction(Sequence::create(Actions));
    };
}

void RocketNode::refreshChildNodes() {
    removeAllChildren();
    Size CS = getContentSize();
    
    if (true) {
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
    }
}

END_NS_STARFALL
