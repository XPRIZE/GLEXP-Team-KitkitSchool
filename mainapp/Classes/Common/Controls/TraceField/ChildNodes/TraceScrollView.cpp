//
//  TraceScrollView.cpp -- Scroller for the case when the string is too long to be presented
//  TodoSchool on May 2, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "TraceScrollView.h"
#include "../Utils/TraceFieldDepot.h"
#include <Games/NumberTrace/Common/Repr/AllRepr.h>


BEGIN_NS_TRACEFIELD

bool TraceScrollView::init() {
    if (!Super::init()) { return false; }

    clearInternals();

    return true;
}


void TraceScrollView::clearInternals() {
    setTouchEnabled(false);
    setSwallowTouches(false);
    removeScrollBar();
    
    ScrollStartPosition = Point::ZERO;
    ScrollStopPosition = Point::ZERO;
    ScrollClock = 0.f;
    
    TheTraceString.OnValueUpdate = [this](TraceString&) {
        TheScrollCache.reset();
    };
    
    ClippingEnabled.OnValueUpdate = [this](bool&) {
        setClippingEnabled(ClippingEnabled());
    };
}

void TraceScrollView::onEnter() {
    Super::onEnter();
    scheduleUpdate();
}

void TraceScrollView::onExit() {
    Super::onExit();
    unscheduleUpdate();
}

void TraceScrollView::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);

    size_t GI = PassedIndex().GlyphIndex;
    if (!TheScrollCache || *TheScrollCache != GI) {
        // NB(xenosoz, 2016): Dirty. Set a new goal here.
        if (TheStyle().ScrollLetterByLetter) {
            scrollTheGlyphToRegion();
        }
        else {
            scrollThisToCenter();
            finishTheScrollNow();
        }
        
        // NB(xenosoz, 2016): Set it clean.
        TheScrollCache.reset(GI);
    }

    const float ScrollTimeLimit = .3f;
    if (ScrollClock < ScrollTimeLimit) {
        // NB(xenosoz, 2016): Update inner container position.
        ScrollClock += DeltaSeconds;

        // NB(xenosoz, 2016): T is in [0, 1].
        float T = pow(min(ScrollClock, ScrollTimeLimit) / ScrollTimeLimit, .3f);
        Point CP = ScrollStartPosition * (1.f - T) + ScrollStopPosition * T;
        
        setInnerContainerPosition(CP);
    }
}

void TraceScrollView::scrollThisToZero() {
    ScrollStartPosition = getInnerContainerPosition();
    ScrollStopPosition = Point::ZERO;
    ScrollClock = 0.f;
}

void TraceScrollView::scrollThisToBegin() {
    TraceFieldDepot Depot;
    Size CS = getContentSize();
    Rect BB = TheTraceString().roughBoundingBox();

    Point BeginCamera = ([&] {
        Point It(0.f, BB.midY() - CS.height/2.f);
        if (TheTraceString().size() > 0) {
            Point P = TheTraceString().minPointForGlyphAt(0);
            It.x += (P.x - Depot.cursorMaxRadius());
        }
        return It;
    }());

    ScrollStartPosition = getInnerContainerPosition();
    ScrollStopPosition = -BeginCamera;
    ScrollClock = 0.f;
}

void TraceScrollView::scrollThisToEnd() {
    TraceFieldDepot Depot;
    Size CS = getContentSize();
    Rect BB = TheTraceString().roughBoundingBox();
    
    Point EndCamera = ([&] {
        Point It(-CS.width, BB.midY() - CS.height/2.f);
        if (TheTraceString().size() > 0) {
            Point P = TheTraceString().maxPointForGlyphAt(TheTraceString().size() - 1);
            It.x += (P.x + Depot.cursorMaxRadius());
        }
        return It;
    }());
    
    ScrollStartPosition = getInnerContainerPosition();
    ScrollStopPosition = -EndCamera;
    ScrollClock = 0.f;
}

void TraceScrollView::scrollThisToCenter() {
    Size CS = getContentSize();
    Rect BB = (UseTightBoundingBox() ?
               TheTraceString().tightBoundingBox() : TheTraceString().roughBoundingBox());

    Point CenterCamera(BB.midX() - CS.width/2.f, BB.midY() - CS.height/2.f);

    ScrollStartPosition = getInnerContainerPosition();
    ScrollStopPosition = -CenterCamera;
    ScrollClock = 0.f;
}
    
void TraceScrollView::scrollTheGlyphToCenter() {
    if (PassedIndex().WorldIndex > 0) { return; }
    
    size_t GI = PassedIndex().GlyphIndex;
    if (GI >= TheTraceString().size()) { return; }
    
    Size CS = getContentSize();
    Point CenterCamera = TheTraceString().centerForGlyphAt(GI);
    CenterCamera.x -= CS.width / 2.f;
    CenterCamera.y -= CS.height / 2.f;
    
    ScrollStartPosition = getInnerContainerPosition();
    ScrollStopPosition = -CenterCamera;
    ScrollClock = 0.f;
}

void TraceScrollView::scrollTheGlyphToRegion() {
    if (PassedIndex().WorldIndex > 0) { return; }
    
    TraceFieldDepot Depot;
    Size CS = getContentSize();
    Rect BB = TheTraceString().roughBoundingBox();

    if (BB.size.width <= CS.width) {
        // NB(xenosoz, 2016): Align to center when letter width is small enough.
        scrollThisToCenter();
        return;
    }

    size_t GI = PassedIndex().GlyphIndex;
    if (GI >= TheTraceString().size()) { return; }

    Point BeginCamera = ([&] {
        Point It(0.f, BB.midY() - CS.height/2.f);
        if (TheTraceString().size() > 0) {
            Point P = TheTraceString().minPointForGlyphAt(0);
            It.x += (P.x - Depot.cursorMaxRadius());
        }
        return It;
    }());

    Point EndCamera = ([&] {
        Point It(-CS.width, BB.midY() - CS.height/2.f);
        if (TheTraceString().size() > 0) {
            Point P = TheTraceString().maxPointForGlyphAt(TheTraceString().size() - 1);
            It.x += (P.x + Depot.cursorMaxRadius());
        }
        return It;
    }());

    Point CenterCamera = TheTraceString().centerForGlyphAt(GI);
    CenterCamera.x -= CS.width / 2.f;
    CenterCamera.y -= CS.height / 2.f;

    float Y = BB.midY() - CS.height / 2.f;
    Point Camera(min(max(BeginCamera.x, CenterCamera.x), EndCamera.x), Y);
    
    ScrollStartPosition = getInnerContainerPosition();
    ScrollStopPosition = -Camera;
    ScrollClock = 0.f;
}

void TraceScrollView::finishTheScrollNow() {
    Point CP = ScrollStopPosition;
    ScrollStartPosition = CP;
    setInnerContainerPosition(CP);
}

END_NS_TRACEFIELD
