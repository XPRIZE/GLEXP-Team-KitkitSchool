//
//  AnswerPad.cpp -- Empty/full slot for SpellBall
//  TodoSchool on Aug 22, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "AnswerPad.h"
#include "../Utils/SpellingDepot.h"


BEGIN_NS_SPELLING;


namespace {
    bool showDebugLog() { return false; }

    float scoreForBallToPutInSlot(SpellBall* Ball, BallSlot* Slot)
    {
        const float Inf = numeric_limits<float>::infinity();

        if (!Ball || !Slot) { return -Inf; }
        if (Ball->BallClass() != Slot->SlotClass()) { return -Inf; }
        if (Slot->filled()) { return -Inf; }

        // NB(xenosoz, 2016): Both ball and slot are circles.
        float Dist = ([&] {
            Point SP = Slot->convertToWorldSpace(Slot->getContentSize() / 2.f);
            Point BP = Ball->convertToWorldSpace(Ball->getContentSize() / 2.f);
            return SP.distance(BP);
        }());
        
        auto radius = [](Node* N) {
            Size CS = N->getContentSize();
            Point O = N->convertToWorldSpace(N->getContentSize() / 2.f);
            Point A = N->convertToWorldSpace(Point(N->getContentSize() / 2.f) + Vec2(CS.width, 0));

            return O.distance(A);
        };

        float BallR = radius(Ball);
        float SlotR = radius(Slot);
        float SumR = BallR + SlotR;
        
        float Score = max(0.f, (SumR - Dist) / SumR);
        if (showDebugLog()) {
            REPR(BallR);
            REPR(SlotR);
            REPR(SumR);
            REPR(Dist);
            REPR(Score);
        }

        return Score;
    }
}  // unnamed namespace


bool AnswerPad::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();

    return true;
}

size_t AnswerPad::size() const {
    return Slots.size();
}

Point AnswerPad::slotPositionWithIndex(size_t Index) const {
    if (Index >= size()) {
        CCLOGERROR("Index(%zu) is out of size(%zu) in %s", Index, size(), __PRETTY_FUNCTION__);
        return Point::ZERO;
    }
    
    return Slots[Index]->getPosition();
}

void AnswerPad::fillSlotWithIndex(size_t Index) {
    if (Index >= size()) {
        CCLOGERROR("Index(%zu) is out of size(%zu) in %s", Index, size(), __PRETTY_FUNCTION__);
        return;
    }
    
    Slots[Index]->fillSlot();
    checkForCompletion();
}

void AnswerPad::tryToCatchBall(SpellBall* Ball) {
    if (!Ball) { return; }

    BallSlot* BestSlot = nullptr;
    float BestScore = -numeric_limits<float>::infinity();
    
    for (auto Slot : Slots) {
        float Score = scoreForBallToPutInSlot(Ball, Slot);
        
        if (showDebugLog()) { CCLOG("> %f", Score); }
        if (Score < 0.80f) { continue; }
        
        if (BestScore <= Score) {
            BestSlot = Slot;
            BestScore = Score;
        }
    }
    
    if (showDebugLog()) { CCLOG("! %f", BestScore); }
    if (!BestSlot) { return; }
    
    SpellingDepot().soundForCardHit().play();
    BestSlot->fillSlot();
    Ball->removeFromParent();
    
    checkForCompletion();
}


void AnswerPad::clearInternals() {
    TheWork.OnValueUpdate = [this](Work&) {
        refreshChildNodes();
    };
}

void AnswerPad::refreshChildNodes() {
    removeAllChildren();
    Slots.clear();
    
    Size GameSize = SpellingDepot().gameSize();
    Size SlotSize = BallSlot::defaultSize();
    setContentSize(Size(GameSize.width, SlotSize.height));
    Size CS = getContentSize();

    string WorkWord = TheWork().Word;
    for (size_t I = 0, E = WorkWord.size(); I < E; ++I) {
        char C = WorkWord[I];
        float RatioX = (I - (WorkWord.size() - 1) / 2.f);

        auto It = BallSlot::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(SlotSize.width * RatioX + CS.width / 2.f, CS.height / 2.f));

        It->SlotClass.update(string{C});
        addChild(It);
        Slots.push_back(It);
    }
}

void AnswerPad::checkForCompletion() {
    for (auto Slot : Slots) {
        if (!Slot->filled()) { return; }
    }
    
    auto Guard = ScopeGuard([this] { retain(); },
                            [this] { release(); });
    
    if (OnAnswerDidComplete)
        OnAnswerDidComplete();
}

END_NS_SPELLING;
