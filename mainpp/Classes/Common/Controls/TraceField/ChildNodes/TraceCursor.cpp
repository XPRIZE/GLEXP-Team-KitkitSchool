//
//  TraceCursor.cpp -- The trace cursor with arrow sign
//  TodoSchool on Apr 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceCursor.h"
#include "../Utils/TraceFieldDepot.h"


BEGIN_NS_TRACEFIELD

TraceCursor* TraceCursor::create() {
    TraceFieldDepot Depot;
    return createWithFilename(Depot.cursorFilename());
}

TraceCursor* TraceCursor::createWithFilename(const string& Filename)
{
    unique_ptr<TraceCursor> Ret(new(std::nothrow) TraceCursor());

    if (Ret && Ret->initWithFilename(Filename))
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    
    return Ret.release();
}

bool TraceCursor::initWithFilename(const string& Filename) {
    if (!Super::initWithFile(Filename))
        return false;

    clearInternals();

    return true;
}


void TraceCursor::clearInternals() {
    Enabled.OnValueUpdate = [this](bool&) {
        setVisible(Enabled());
    };
}

void TraceCursor::onEnter() {
    Super::onEnter();
    this->scheduleUpdate();
}

void TraceCursor::onExit() {
    Super::onExit();
    this->unscheduleUpdate();
}

void TraceCursor::update(float DeltaSeconds) {
    if (CursorVelocity().length() < 1e-9) {
        // NB(xenosoz, 2016): Try to avoid 0/0.
        setVisible(false);
        return;
    }

    Point CursorPoint = CursorPointInGlyphNode.value();
    setPosition(CursorPoint);

    float Scale = (CursorPicked.value() ? PickedScale() : NormalScale());
    setScale(Scale);

    float Angle = atan2(CursorVelocity.value().y, CursorVelocity.value().x);
    setRotation(-Angle * 180.f / (float) M_PI);

    setVisible(Enabled());
}
    
END_NS_TRACEFIELD
