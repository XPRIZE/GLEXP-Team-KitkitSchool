//
//  WoodSlot.cpp on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodSlot.h"
#include "../Utils/WoodenPuzzleDepot.h"


BEGIN_NS_WOODENPUZZLES;

namespace {
    int localZOrderForPiece() { return 10; }
    int localZOrderForSlot() { return 0; }
    
    float durationForDummyMove() { return .2f; }
}  // unnamed namespace


WoodSlot::WoodSlot()
    : SlotSprite(nullptr)
    , TheDummyPiece(nullptr)
{
}

bool WoodSlot::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

void WoodSlot::clearSlot() {
    if (SlotSprite)
        SlotSprite->setVisible(true);
    
    if (TheDummyPiece)
        TheDummyPiece->setVisible(false);
}

void WoodSlot::fillSlot() {
    if (SlotSprite)
        SlotSprite->setVisible(true);
    
    if (TheDummyPiece) {
        TheDummyPiece->switchTo2D();
        TheDummyPiece->setVisible(true);
    }
    
    WoodenPuzzleDepot(Mode()).soundForCardHit().play();
}

void WoodSlot::pullPieceIllusion(Point PositionInWorldSpace) {
    if (!TheDummyPiece) { return; }
    
    Size CS = getContentSize();
    auto DParent = TheDummyPiece->getParent();
    if (DParent)
        TheDummyPiece->setPosition(DParent->convertToNodeSpace(PositionInWorldSpace));

    // NB(xenosoz, 2016): Go animation!
    TheDummyPiece->switchTo3D();
    TheDummyPiece->setVisible(true);

    Vector<FiniteTimeAction*> Actions;
    Actions.pushBack(MoveTo::create(durationForDummyMove(), CS / 2.f));
    Actions.pushBack(CallFunc::create([this] {
        TheDummyPiece->switchTo2D();
        WoodenPuzzleDepot(Mode()).soundForCardHit().play();
    }));

    TheDummyPiece->runAction(Sequence::create(Actions));
}

void WoodSlot::clearInternals() {
    Mode.OnValueUpdate =
    SlotID.OnValueUpdate =
    FaceSkin.OnValueUpdate =
    DepthSkin.OnValueUpdate =
    ShadowSkin.OnValueUpdate =
    SlotSkin.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void WoodSlot::refreshChildNodes() {
    removeAllChildren();
    
    TheDummyPiece = ([&] {
        auto It = DummyPiece::create();
        It->PieceID.update(SlotID());
        It->FaceSkin.follow(FaceSkin);
        It->DepthSkin.follow(DepthSkin);
        It->ShadowSkin.follow(ShadowSkin);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(It->getContentSize() / 2.f);
        It->setVisible(false);
        
        addChild(It, localZOrderForPiece());
        setContentSize(It->getContentSize());
        return It;
    }());

    Size CS = getContentSize();

    SlotSprite = ([&] {
        auto It = Sprite::create(SlotSkin());
        if (!It) { return (It = nullptr); }

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It, localZOrderForSlot());
        return It;
    }());
}

END_NS_WOODENPUZZLES;
