//
//  CountField.cpp on Sep 2, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "CountField.h"
#include "../Utils/MainDepot.h"

#include <Games/NumberTrace/Common/CountField/ChildNodes/Bee.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/BlueButterfly.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/Moth.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/YellowButterfly.h>

#include <Games/NumberTrace/Common/CountField/ChildNodes/Ant.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/Beetle.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/Cockroach.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/Ladybug.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/Spider.h>
#include <Games/NumberTrace/Common/CountField/ChildNodes/StagBeetle.h>


BEGIN_NS_MOVINGINSECTS

using namespace todoschool::countfield;

namespace {
    bool useSecondInsectSet() { return true; }

    Pose beginPoseForIndex(Size CS, size_t Count, size_t Index) {
        float Rotation = MATH_DEG_TO_RAD(random(90 - 15, 90 + 15));
        return Pose(-Point(CS), Rotation);
    }
    Pose pausePoseForIndex(Size CS, size_t Count, size_t Index) {
        float X = CS.width * (Index + 1) / (Count + 1);
        float Y = CS.height * (Index % 2 == 0 ? 1 : 2) / 3;
        float Rotation = MATH_DEG_TO_RAD(random(90 - 15, 90 + 15));
        
        X += random(-30.f, +30.f);
        Y += random(-30.f, +30.f);

        return Pose(Point(X, Y), Rotation);
    }
    Pose endPoseForIndex(Size CS, size_t Count, size_t Index) {
        float Rotation = MATH_DEG_TO_RAD(random(90 - 15, 90 + 15));
        return Pose(Point(CS) * 2, Rotation);
    }
    
    int localZOrderForObjectNode() { return 10; }
    int localZOrderForShadowNode() { return -10; }
}  // unnamed namespace


bool CountField::init() {
    if (!Super::init()) { return false; }
    
    clearInternals();
    refreshChildNodes();
    
    return true;
}

void CountField::appendGroups(size_t CountA, size_t CountB, float Duration) {
    appendGroupA(CountA, CountB, Duration);
    appendGroupB(CountA, CountB, Duration);
}

void CountField::appendGroupA(size_t CountA, size_t CountB, float Duration) {
    Size CS = getContentSize();
    size_t Count = CountA + CountB;

    for (auto ObjectID = 0; ObjectID < CountA; ++ObjectID) {
        size_t Index = ObjectID;

        auto It = ObjectCreatorA();
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(0.f, 0.f));
        It->jumpTo(beginPoseForIndex(CS, Count, Index));
        It->moveIn(pausePoseForIndex(CS, Count, Index),
                   Duration * (1.f + random(-4, 0) * .125f),
                   Duration * 0.50f);

        GroupA.push_back(It);
        addChild(It,localZOrderForObjectNode());
    }
}

void CountField::appendGroupB(size_t CountA, size_t CountB, float Duration) {
    Size CS = getContentSize();
    size_t Count = CountA + CountB;
    
    for (auto ObjectID = 0; ObjectID < CountB; ++ObjectID) {
        size_t Index = CountA + ObjectID;

        auto It = ObjectCreatorB();
        It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        It->setPosition(Point(0.f, 0.f));
        It->jumpTo(beginPoseForIndex(CS, Count, Index));
        It->moveIn(pausePoseForIndex(CS, Count, Index),
                   Duration * (1.f + random(-4, 0) * .125f),
                   Duration * 0.50f);
        
        GroupB.push_back(It);
        addChild(It, localZOrderForObjectNode());
    }
}

void CountField::removeGroups(float Duration) {
    removeGroupA(Duration);
    removeGroupB(Duration);
}

void CountField::removeGroupA(float Duration) {
    Size CS = getContentSize();
    size_t Count = GroupA.size() + GroupB.size();

    for (size_t I = 0, E = GroupA.size(); I < E; ++I) {
        auto Object = GroupA[I];
        Object->moveOut(endPoseForIndex(CS, Count, I),
                        Duration,
                        Duration * 0.50f);
        
        auto Shadow = ShadowCreator();
        Shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        // XXX: Can't we just call it as `Object->getPosition()`?
        Shadow->setPosition(Object->getPosition() + Object->ThePose().Position);
        Shadow->setRotation(Object->getRotation() +
                            MATH_RAD_TO_DEG(M_PI_2 - Object->ThePose().Rotation));
        addChild(Shadow, localZOrderForShadowNode());
    }
}

void CountField::removeGroupB(float Duration) {
    Size CS = getContentSize();
    size_t Count = GroupA.size() + GroupB.size();
    
    for (size_t I = 0, E = GroupB.size(); I < E; ++I) {
        auto Object = GroupB[I];
        Object->moveOut(endPoseForIndex(CS, Count, I),
                        Duration,
                        Duration * 0.50f);

        auto Shadow = ShadowCreator();
        Shadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        // XXX: Can't we just call it as `Object->getPosition()`?
        Shadow->setPosition(Object->getPosition() + Object->ThePose().Position);
        Shadow->setRotation(Object->getRotation() +
                            MATH_RAD_TO_DEG(M_PI_2 - Object->ThePose().Rotation));
        addChild(Shadow, localZOrderForShadowNode());
    }
}

void CountField::shakeGroups(float Duration) {
    shakeGroupA(Duration);
    shakeGroupB(Duration);
}

void CountField::shakeGroupA(float Duration) {
    for (auto Obj : GroupA) {
        auto It = Obj;

        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(CallFunc::create([It] { It->Moving.update(true); }));
        Actions.pushBack(DelayTime::create(.8f));
        Actions.pushBack(CallFunc::create([It] { It->Moving.update(false); }));

        It->stopAllActions();
        It->runAction(Sequence::create(Actions));
    }
}

void CountField::shakeGroupB(float Duration) {
    for (auto Obj : GroupB) {
        auto It = Obj;
        
        Vector<FiniteTimeAction*> Actions;
        Actions.pushBack(CallFunc::create([It] { It->Moving.update(true); }));
        Actions.pushBack(DelayTime::create(.8f));
        Actions.pushBack(CallFunc::create([It] { It->Moving.update(false); }));

        It->stopAllActions();
        It->runAction(Sequence::create(Actions));
    }
}

void CountField::clearInternals() {
    array<function<CountObject*()>, 10> ObjectCreatorsA = {
        // NB(xenosoz, 2016): Air nodes
        [] { return Bee::create(0); },
        [] { return BlueButterfly::create(0); },
        [] { return Moth::create(0); },
        [] { return YellowButterfly::create(0); },
        
        // NB(xenosoz, 2016): Ground nodes
        [] { return Ant::create(0); },
        [] { return Beetle::create(0); },
        [] { return Cockroach::create(0); },
        [] { return Ladybug::create(0); },
        [] { return Spider::create(0); },
        [] { return StagBeetle::create(0); },
    };

    array<function<CountObject*()>, 10> ObjectCreatorsB = {
        // NB(xenosoz, 2016): Air nodes
        [] { return Bee::create(1); },
        [] { return BlueButterfly::create(1); },
        [] { return Moth::create(1); },
        [] { return YellowButterfly::create(1); },
        
        // NB(xenosoz, 2016): Ground nodes
        [] { return Ant::create(1); },
        [] { return Beetle::create(1); },
        [] { return Cockroach::create(1); },
        [] { return Ladybug::create(1); },
        [] { return Spider::create(1); },
        [] { return StagBeetle::create(1); },
    };

    string P = MainDepot().assetPrefix() + "/Shadows";

    array<function<Sprite*()>, 10> ShadowCreators = {
        [P] { return Sprite::create(P + "/bee_normal_0001.png"); },
        [P] { return Sprite::create(P + "/BF2_normal_0001.png"); },
        [P] { return Sprite::create(P + "/moth_normal_0001.png"); },
        [P] { return Sprite::create(P + "/BF_normal_0001.png"); },
        
        [P] {
            auto It = Sprite::create(P + "/ant_normal_0001.png");
            It->setScale(3.f / 4.f);  // XXX: It's better to resize the design resource itself.
            return It;
        },

        [P] { return Sprite::create(P + "/beetle_normal_0001.png"); },
        [P] {
            auto It = Sprite::create(P + "/cockroach_normal_0001.png");
            It->setScale(2.f / 3.f);  // XXX: It's better to resize the design resource itself.
            return It;
        },

        [P] {
            auto It = Sprite::create(P + "/ladybug_normal_0001.png");
            It->setScale(3.f / 4.f);  // XXX: It's better to resize the design resource itself.
            return It;
        },

        [P] { return Sprite::create(P + "/spider_normal_0001.png"); },
        [P] { return Sprite::create(P + "/SB_normal_0001.png"); },
    };
    

    size_t MinSize = min(ObjectCreatorsA.size(),
                         min(ObjectCreatorsB.size(),
                             ShadowCreators.size()));
    size_t Index = random<size_t>(0, MinSize - 1);
    ObjectCreatorA = ObjectCreatorsA[Index];

    if (useSecondInsectSet())
        ObjectCreatorB = ObjectCreatorsB[Index];
    else
        ObjectCreatorB = ObjectCreatorsA[Index];

    ShadowCreator = ShadowCreators[Index];


    // NB(xenosoz, 2016): Unload unused textures.
    Director::getInstance()->getTextureCache()->removeUnusedTextures();

    // NB(xenosoz, 2016): A shotgun approach to heat the cache up.
    if (ObjectCreatorA) { ObjectCreatorA(); }
    if (ObjectCreatorB) { ObjectCreatorB(); }
    if (ShadowCreator) { ShadowCreator(); }
}

void CountField::refreshChildNodes() {
    removeAllChildren();

    GroupA.clear();
    GroupB.clear();
}

END_NS_MOVINGINSECTS
