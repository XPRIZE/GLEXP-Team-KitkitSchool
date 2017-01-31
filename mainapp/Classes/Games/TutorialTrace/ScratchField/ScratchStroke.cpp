//
//  ScratchStroke.cpp on Dec 27, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "ScratchStroke.h"


BEGIN_NS_TUTORIALTRACE

ScratchStroke::ScratchStroke()
: BotNode(nullptr)
, MaskNode(nullptr)
, ClipNode(nullptr)
, TopNode(nullptr)
{
}

bool ScratchStroke::init() {
    return init("", "", 0, 0, 0);
}

bool ScratchStroke::init(const string& Top, const string& Bot,
                         int Past, int Present, int Future)
{
    if (!Super::init()) { return false; }
    
    this->LocalZIndexForPast = Past;
    this->LocalZIndexForPresent = Present;
    this->LocalZIndexForFuture = Future;
    
    BotNode = [&] {
        auto It = Sprite::create(Bot);
        auto CS = It->getContentSize();
        setContentSize(CS);

        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        addChild(It);
        return It;
    }();


    auto CS = getContentSize();

    MaskNode = [&] {
        // NB(xenosoz, 2016): The mask is *not* retained here. See ClipNode section.
        auto It = DrawNode::create();
        It->setContentSize(CS * 1.02f);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS * 1.02f / 2.f);

        return It;
    }();
    
    ClipNode = [&] {
        // NB(xenosoz, 2016): The mask is retained here.
        auto It = ClippingNode::create(MaskNode);
        It->setContentSize(CS);
        It->setInverted(true);
        
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);
        
        [&] {
            auto S = Sprite::create(Top);
            S->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            S->setPosition(CS / 2.f);
            It->addChild(S);
        }();

        addChild(It);
        return It;
    }();
    
    TopNode = [&] {
        auto It = Sprite::create(Top);
        It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        It->setPosition(CS / 2.f);

        addChild(It);
        return It;
    }();
    
    PassedIndex.OnValueUpdate = [this](TraceIndex&) {
        TraceIndex BaseBegin = BaseIndex().indexForStrokeBegin(TheTraceString());
        TraceIndex BaseEnd = BaseIndex().indexForStrokeEnd(TheTraceString());

        bool Past = (BaseEnd <= PassedIndex());
        bool Present = (BaseBegin <= PassedIndex() && PassedIndex() < BaseEnd);
        bool Future = (PassedIndex() < BaseBegin);

        if (TopNode) { TopNode->setVisible(Future); }
        if (ClipNode) { ClipNode->setVisible(Present); }
        if (BotNode) { BotNode->setVisible(Past || Present); }
        
        if (Past) { setLocalZOrder(LocalZIndexForPast); }
        if (Present) { setLocalZOrder(LocalZIndexForPresent); }
        if (Future) { setLocalZOrder(LocalZIndexForFuture); }
    };

    return true;
}

void ScratchStroke::scratchCircle(Point P, float Radius) {
    if (!MaskNode) { return; }
    MaskNode->drawSolidCircle(P, Radius, 0, 200, Color4F::MAGENTA);
}

void ScratchStroke::scratchSegments(const vector<Point>& PS, float Radius) {
    if (!MaskNode) { return; }
    MaskNode->drawSegments(&PS[0], (int)PS.size(), Radius, Color4B::MAGENTA);
}

END_NS_TUTORIALTRACE
