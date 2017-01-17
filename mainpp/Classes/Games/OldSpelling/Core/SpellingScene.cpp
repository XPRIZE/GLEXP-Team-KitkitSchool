//
//  SpellingScene.cpp on Aug 10, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "SpellingScene.h"
#include "../ChildNodes/BallBound.h"
#include "../Utils/SpellingDepot.h"
#include "../MonkeyPatches/SmoothPhysicsWorld.h"
#include <Common/Basic/SoundEffect.h>
#include <Common/Controls/TodoSchoolBackButton.hpp>


BEGIN_NS_SPELLING;


namespace {
    float physicsSpeed() { return 3.f; }
    float physicsUpdateRate() { return .1f; }

    string contentSkin() {
        return SpellingDepot().assetPrefix() + "/Background/spelling_background.png";
    }
}  // unnamed namespace


SpellingScene* SpellingScene::createScene() {
    auto Impl = SpellingScene::create();
    
    return Impl;
}

SpellingScene::SpellingScene()
    : TheGameNode(nullptr)
    , TheImageCard(nullptr)
    , TheAnswerPad(nullptr)
    , ReplacedPhysicsWorldFromParent(nullptr)
{
}

SpellingScene::~SpellingScene() {
    revertPhysicsWorld();
}


bool SpellingScene::init() {
    // NB(xenosoz, 2016): I feel guilty calling init*() twice here,
    //   but Scene::initWithPhysics() doesn't seem to conflict with Scene::init().
    if (!Super::init()) { return false; }
    if (!Super::initWithPhysics()) { return false; }
    
    SpellingDepot().preloadSoundEffects();

    replacePhysicsWorld();
    clearInternals();
    refreshChildNodes();
    
    return true;
}


void SpellingScene::clearInternals() {
    auto PWorld = getPhysicsWorld();

    if (PWorld) {
        PWorld->setSpeed(physicsSpeed());
        PWorld->setUpdateRate(physicsUpdateRate());
    }
    
    TheWork.OnValueUpdate = [this](Work&) {
        refreshChildNodes();
    };
}

void SpellingScene::refreshChildNodes() {
    removeAllChildren();
    
    SpellingDepot Depot;
    Size WindowSize = Depot.windowSize();
    Size GameSize = Depot.gameSize();
    
    [&] {
        // NB(xenosoz, 2016): The background node.
        auto BN = Node::create();
        BN->setContentSize(WindowSize);
        
        if (true) {
            auto It = Sprite::create(contentSkin());
            Size SpriteSize = It->getContentSize();
            float Scale = Depot.scaleToCoverWindow(SpriteSize);
            
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(WindowSize/2.f);
            
            It->setScale(Scale);
            It->setBlendFunc(BlendFunc::DISABLE);
            BN->addChild(It);
        }
        
        addChild(BN);
    }();


    TheGameNode = ([&] {
        float Scale = Depot.scaleToBeContainedInWindow(GameSize);
        
        Node* It = Node::create();
        It->setContentSize(GameSize);
        It->setScale(Scale);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        It->setPosition(Point(WindowSize.width/2.f, 0.f));
        
        addChild(It);
        return It;
    }());
    

    [&] {
        // NB(xenosoz, 2016): The back button.
        TodoSchoolBackButton* It = TodoSchoolBackButton::create();
        It->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        It->setPosition(Point(25.f, WindowSize.height - 25.f));
        
        
        addChild(It);
    }();
    

    // NB(xenosoz, 2016): Descendants of GameNode
    
    TheImageCard = ([&] {
        auto It = ImageCard::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize.width / 2.f, GameSize.height * 3.f / 4.f));
        
        It->TheWork.follow(TheWork);

        TheGameNode->addChild(It);
        return It;
    }());
    
    TheAnswerPad = ([&] {
        auto It = AnswerPad::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Point(GameSize.width / 2.f, GameSize.height * 2.f / 4.f));
        
        It->TheWork.follow(TheWork);
        It->OnAnswerDidComplete = [this] {
            float Delay = 1.f;
            scheduleOnce([this](float) {  // NB(xenosoz, 2016): For stage transition
                beginNextWork();
            }, Delay, "SpellingScene::handleAnswerDidComplete");
        };
        
        TheGameNode->addChild(It);
        return It;
    }());
    
    [&] {
        auto It = BallBound::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(GameSize / 2.f);
        
        TheGameNode->addChild(It);
        return It;
    }();

    // NB(xenosoz, 2016): Aux function for create a spell ball.
    auto createBall = [&](const string& Letter,
                          const Point& Position) {
        auto It = SpellBall::create();
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(Position);
        
        It->BallClass.update(Letter);
        It->OnDrag = [this, It](Touch* T, Event* E) {
            if (!TheAnswerPad) { return; }
            TheAnswerPad->tryToCatchBall(It);
        };
        return It;
    };

    [&] {
        // NB(xenosoz, 2016): The answer balls. At least BallRatio portion of balls are generated.
        if (!TheAnswerPad) { return; }

        const float BallRatio = random(0.20f, 0.60f);
        size_t RemainSlotCount = TheAnswerPad->size();
        size_t RemainBallCount = (size_t)ceilf(RemainSlotCount * BallRatio);

        for (size_t I = 0, E = TheAnswerPad->size(); I < E; ++I) {
            bool MakeBall = random(0.f, 1.f - FLT_EPSILON) < ((float)RemainBallCount / RemainSlotCount);
            RemainSlotCount -= 1;
            RemainBallCount -= (MakeBall ? 1 : 0);
            
            if (!MakeBall) {
                TheAnswerPad->fillSlotWithIndex(I);
                continue;
            }

            string Letter{TheWork().Word[I]};
            Point Position(TheAnswerPad->slotPositionWithIndex(I));

            auto It = createBall(Letter, Position);
            TheGameNode->addChild(It);
        }
    }();

    [&] {
        // NB(xenosoz, 2016): Other random balls.
        for (size_t I = 0; I < 6; I += 1) {
            string Letter{random('a', 'z')};
            Point Position(random(0.f, GameSize.width), random(0.f, GameSize.height));

            auto It = createBall(Letter, Position);
            TheGameNode->addChild(It);
        }
    }();
}

void SpellingScene::onEnter() {
    Super::onEnter();
    
    beginFirstWork();
}

void SpellingScene::replacePhysicsWorld() {
    assert(!ReplacedPhysicsWorldFromParent);
    
    ReplacedPhysicsWorldFromParent = _physicsWorld;
    _physicsWorld = SmoothPhysicsWorld::construct(this);
}

void SpellingScene::revertPhysicsWorld() {
    assert(ReplacedPhysicsWorldFromParent);
    
    auto SPWorld = dynamic_cast<SmoothPhysicsWorld*>(_physicsWorld);
    if (SPWorld) {
        delete SPWorld;
        _physicsWorld = nullptr;
    }
    
    _physicsWorld = ReplacedPhysicsWorldFromParent;
    ReplacedPhysicsWorldFromParent = nullptr;
}

void SpellingScene::beginFirstWork() {
    TheWorkIndex = 0;
    prepareCurrentWork();
}

void SpellingScene::beginNextWork() {
    TheWorkIndex += 1;
    prepareCurrentWork();
}

void SpellingScene::prepareCurrentWork() {
    vector<Work> WorkList = {
        Work("en.1", "bun"),
        Work("en.1_2", "bug"),
        Work("en.2", "mug"),
        Work("en.3", "jug"),
        Work("en.3_2", "hut"),
    };

    TheWork.update(WorkList[TheWorkIndex % WorkList.size()]);  // XXX
}

END_NS_SPELLING;
