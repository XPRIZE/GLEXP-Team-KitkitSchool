//
//  GameBoard.h on Aug 11, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "WoodenPuzzlesGameBoard.h"
#include "../Utils/WoodenPuzzleDepot.h"


BEGIN_NS_WOODENPUZZLES;


namespace {
    int localZOrderForUpperSlot() { return 10; }
    int localZOrderForLowerSlot() { return 5; }
}  // unnamed namespace


bool GameBoard::init() {
    if (!Super::init()) { return false; }

    clearInternals();
    refreshChildNodes();

    return true;
}

void GameBoard::appendWoodSlot(WoodSlot* Slot) {
    if (!Slot) { return; }
    Slot->setLocalZOrder(localZOrderForLowerSlot());

    addChild(Slot);
    WoodSlots.insert({Slot->SlotID(), Slot});
}

void GameBoard::promoteWoodSlot(WoodSlot* Slot) {
    if (!Slot) { return; }

    for (auto Item : WoodSlots) {
        auto S = Item.second;
        if (!S) { continue; }
        S->setLocalZOrder(localZOrderForLowerSlot());
    }
    Slot->setLocalZOrder(localZOrderForUpperSlot());
}

void GameBoard::clearInternals() {
    ContentSkinPath.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
}

void GameBoard::refreshChildNodes() {
    removeAllChildren();
    WoodSlots.clear();

    [&] {
        // NB(xenosoz, 2016): The background. It decides the content size of this node.
        auto It = Sprite::create(ContentSkinPath());
        if (!It) {
            setContentSize(Size::ZERO);
            return;
        }

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(It->getContentSize() / 2.f);
        
        addChild(It);
        setContentSize(It->getContentSize());
    }();
}


END_NS_WOODENPUZZLES;
