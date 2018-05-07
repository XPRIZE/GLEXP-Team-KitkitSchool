//
//  Platform.cpp -- A stone platform with many answer balls
//  TodoSchool on Sep 13, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "Platform.h"
#include "../BallNodes/AnswerBall.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_SPELLING

namespace {
    Size contentSize() { return Size(2502.f, 230.f); }

    string contentSkin() {
        return MainDepot().assetPrefix() + "/Platform/Spelling_platform.png";
    }

    string holeSkin() {
        return MainDepot().assetPrefix() + "/Platform/Spelling_platform_hole.png";
    }
    
    string shadowSkin() {
        return MainDepot().assetPrefix() + "/Platform/Spelling_platform_hole_shadow.png";
    }
}  // unnamed namespace


bool Platform::init(BallSize BS) {
    if (!Super::init()) { return false; }
    
    TheBallSize = BS;
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

size_t Platform::maxCapacity() const {
    // NB(xenosoz, 2016): Regardless the ball size, it's capacity is 9 now.
    return 9;
}

size_t Platform::size() const {
    return Letters().size();
}

Point Platform::positionForSlotInWorldSpace(size_t SlotIndex) const {
    Point LocalP = positionForSlotInLocalSpace(SlotIndex);
    Point WorldP = convertToWorldSpace(LocalP);

    return WorldP;
}

Point Platform::positionForSlotInLocalSpace(size_t SlotIndex) const {
    Size CS = getContentSize();
    BallSize BS = TheBallSize;

    Size BallS = AnswerBall::defaultSizeForPassiveBall(BS);
    float BallSpaceH = BallS.width + 40.f;

    float X = BallSpaceH * (SlotIndex - (size() - 1) / 2.f) + (CS.width / 2.f);
    float Y = 150.f + (BallS.height / 2.f);
    return Point(X, Y);
}

void Platform::hideShadowForBallID(size_t SlotIndex) const {
    if (SlotIndex >= Shadows.size()) { return; }

    auto Shadow = Shadows[SlotIndex];
    Shadow->setVisible(false);
}

void Platform::showShadowForBallID(size_t SlotIndex) const {
    if (SlotIndex >= Shadows.size()) { return; }
    
    auto Shadow = Shadows[SlotIndex];
    Shadow->runAction(Sequence::create(DelayTime::create(.1f),
                                       Show::create(),
                                       nullptr));
}

FiniteTimeAction* Platform::actionForEnter2(function<void()> Callback /* = nullptr */) {
    Size GameSize = MainDepot().gameSize();
    Size CS = getContentSize();
    Vector<FiniteTimeAction*> Actions;
    
    Actions.pushBack([&] {
        Vector<FiniteTimeAction*> It;
        It.pushBack(MoveTo::create(0.f, Point(GameSize.width / 2.f, CS.height * (-3))));
        It.pushBack(FadeOut::create(0.f));
        return Spawn::create(It);
    }());
    
    Actions.pushBack([&] {
        Vector<FiniteTimeAction*> It;
        It.pushBack(MoveTo::create(.4f, Point(GameSize.width / 2.f, 0.f)));
        It.pushBack(FadeIn::create(.4f));
        return Spawn::create(It);
    }());
    
    Actions.pushBack(CallFunc::create([this, Callback] {
        if (!Callback) { return; }
        NodeScopeGuard guard(this);
        Callback();
    }));
    
    return Sequence::create(Actions);
}

FiniteTimeAction* Platform::actionForExit(function<void()> Callback /* = nullptr */) {
    Size GameSize = MainDepot().gameSize();
    Size CS = getContentSize();
    Vector<FiniteTimeAction*> Actions;
    
    Actions.pushBack([&] {
        Vector<FiniteTimeAction*> It;
        It.pushBack(MoveTo::create(.4f, Point(GameSize.width / 2.f, CS.height * (-3))));
        It.pushBack(FadeOut::create(.4f));
        return Spawn::create(It);
    }());
    
    Actions.pushBack(CallFunc::create([this, Callback] {
        if (!Callback) { return; }
        NodeScopeGuard guard(this);
        Callback();
    }));
    
    return Sequence::create(Actions);
}

void Platform::setPosition(float X, float Y) {
    Super::setPosition(X, Y);
    
    NodeScopeGuard Guard(this);
    LocalPosition.update(Point(X, Y));
}

void Platform::setOpacity(GLubyte Opacity) {
    Super::setOpacity(Opacity);

    this->Opacity.update(Opacity);
}

void Platform::clearInternals() {
    setContentSize(contentSize());
    setCascadeOpacityEnabled(true);

    Letters.OnValueUpdate = [this](vector<string>&) {
        refreshChildNodes();
    };
}

void Platform::refreshChildNodes() {
    removeAllChildren();
    Shadows.clear();
    Size CS = getContentSize();
    
    [&] {
        // NB(xenosoz, 2016): The platform background
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        addChild(It);
        return It;
    }();
    
    // NB(xenosoz, 2016): Platform hole
    for (size_t I = 0, E = size(); I < E; ++I) {
        auto It = Sprite::create(holeSkin());
        Point P = positionForSlotInLocalSpace(I);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(P.x, 122.f + 45.f));
        
        addChild(It);
    }

    // NB(xenosoz, 2016): Ball shadow
    for (size_t I = 0, E = size(); I < E; ++I) {
        auto It = Sprite::create(shadowSkin());
        Point P = positionForSlotInLocalSpace(I);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(P.x, 122.f + 45.f));
        
        Shadows.push_back(It);
        addChild(It);
    }
}

END_NS_SPELLING
