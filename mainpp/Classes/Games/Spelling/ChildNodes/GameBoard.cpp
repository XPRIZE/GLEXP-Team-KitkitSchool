//
//  GameBoard.cpp -- Game board with a picture and answer slots on it
//  TodoSchool on Sep 12, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "GameBoard.h"
#include "WordImage.h"
#include "../Utils/MainDepot.h"


BEGIN_NS_SPELLING

namespace {
    string contentSkin() { return MainDepot().assetPrefix() + "/GameBoard/spelling_frame.png"; }
    Size contentSize() { return Size(1327.f, 1316.f); }    
}  // unnamed namespace


bool GameBoard::init() {
    return init(BallSize::Large);
}

bool GameBoard::init(BallSize BS) {
    if (!Super::init()) { return false; }
    
    TheBallSize = BS;
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

size_t GameBoard::size() const {
    return Slots.size();
}

BallSlot* GameBoard::slotAtIndex(size_t Index) const {
    return Slots.at(Index);
}

FiniteTimeAction* GameBoard::actionForEnter(function<void()> Callback /* = nullptr */) {
    Size GameSize = MainDepot().gameSize();
    Size CS = getContentSize();
    Vector<FiniteTimeAction*> Actions;

    Actions.pushBack([&] {
        // NB(xenosoz, 2016): Initial position for animation
        Vector<FiniteTimeAction*> It;
        It.pushBack(MoveTo::create(0.f, Point(GameSize.width + CS.width, GameSize.height - 80.f)));

        if (MainDepot().allowFadeInOutForGameBoard())
            It.pushBack(FadeOut::create(0.f));
        else
            It.pushBack(FadeIn::create(0.f));

        return Spawn::create(It);
    }());

    Actions.pushBack([&] {
        // NB(xenosoz, 2016): The move animation
        Vector<FiniteTimeAction*> It;
        auto Move = MoveTo::create(.4f, Point(GameSize.width / 2.f, GameSize.height - 80.f));
        It.pushBack(EaseIn::create(Move, 2.f));

        if (MainDepot().allowFadeInOutForGameBoard())
            It.pushBack(FadeIn::create(.4f));

        return Spawn::create(It);
    }());
    
    Actions.pushBack(CallFunc::create([this] { SoundForEnter.play(); }));
    Actions.pushBack(CallFunc::create([this, Callback] {
        if (!Callback) { return; }
        NodeScopeGuard Guard(this);
        Callback();
    }));

    return Sequence::create(Actions);
}

FiniteTimeAction* GameBoard::actionForEjectBalls(function<void()> Callback /* = nullptr */) {
    Vector<FiniteTimeAction*> Actions;

    for (size_t I = 0, E = Slots.size(); I < E; ++I) {
        auto& Slot = Slots[I];

        if (TheProblem().letterBecomesChoice(I)) {
            Actions.pushBack(CallFunc::create([this, Slot] {
                Slot->ejectBall();
            }));
        }

        Actions.pushBack(DelayTime::create(.1f));
    }

    Actions.pushBack(CallFunc::create([this, Callback] {
        if (!Callback) { return; }
        NodeScopeGuard Guard(this);
        Callback();
    }));

    return Sequence::create(Actions);
}

FiniteTimeAction* GameBoard::actionForExit(function<void()> Callback /* = nullptr */) {
    Size GameSize = MainDepot().gameSize();
    Size CS = getContentSize();
    Vector<FiniteTimeAction*> Actions;
    
    Actions.pushBack(DelayTime::create(.8f));

    Actions.pushBack([&] {
        Vector<FiniteTimeAction*> It;
        auto Move = MoveTo::create(.4f, Point(0.f - CS.width, GameSize.height - 80.f));
        It.pushBack(EaseOut::create(Move, 2.f));
        
        if (MainDepot().allowFadeInOutForGameBoard())
            It.pushBack(FadeOut::create(.4f));

        return Spawn::create(It);
    }());

    Actions.pushBack(CallFunc::create([this, Callback] {
        if (!Callback) { return; }
        NodeScopeGuard Guard(this);
        Callback();
    }));

    return Sequence::create(Actions);
}


void GameBoard::clearInternals() {
    setContentSize(contentSize());
    setCascadeOpacityEnabled(true);
    
    //SoundForEnter = MainDepot().soundForCardBirth();
    SoundForEnter = SoundEffect::emptyEffect();

    TheProblem.OnValueUpdate = [this](Problem&) {
        refreshChildNodes();
    };
}

void GameBoard::refreshChildNodes() {
    removeAllChildren();
    Slots.clear();
    Size CS = getContentSize();
    
    [&] {
        // NB(xenosoz, 2016): The game board itself
        auto It = Sprite::create(contentSkin());
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(CS / 2.f));
        
        addChild(It);
        return It;
    }();
    
    [&] {
        // NB(xenosoz, 2016): Work image
        auto It = WordImage::create();
        It->TheProblem.follow(TheProblem);

        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(110.f, CS.height - 80.f));

        addChild(It);
        return It;
    }();


    // NB(xenosoz, 2016): Candidate ball slots
    string CorrectAnswer = TheProblem().Word;
    BallSize BS = TheBallSize;
    Size SlotSize = BallSlot::defaultSize(BS);
    float SlotSpaceW = SlotSize.width + 18.f;

    for (size_t I = 0, E = CorrectAnswer.size(); I < E; ++I) {
        auto It = BallSlot::create(BS);
        auto Letter = string{CorrectAnswer[I]};

        It->BallText.update(Letter);
        It->OnSlotDidFill = [this] {
            if (!OnAllSlotDidFill) { return; }
            for (auto Slot : Slots)
                if (Slot->empty()) { return; }

            NodeScopeGuard Guard(this);
            OnAllSlotDidFill();
        };

        float X = SlotSpaceW * (I - (CorrectAnswer.size() - 1) / 2.f) + (CS.width / 2.f);
        float Y = 220.f;
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(X, Y));

        Slots.push_back(It);
        addChild(It);
    }
}

END_NS_SPELLING
