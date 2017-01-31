//
//  TraceField.cpp on Apr 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//


#include "TraceField.h"
#include "LiveModels/TraceIndex.h"


#include "Utils/TraceFieldDepot.h"
#include "Utils/TraceLocator.h"
#include "Utils/TraceModelFactory.h"

#include <Games/NumberTrace/Common/Basic/DeviceSpec.h>
#include <Games/NumberTrace/Common/Basic/ScopeGuard.h>
#include <Games/NumberTrace/Common/Utils/TouchEventRepeater.h>
#include <cocos/cocos2d.h>

#include <Games/NumberTrace/Common/Repr/AllRepr.h>


BEGIN_NS_TRACEFIELD

namespace {
    bool showDebugLog() { return false; }
}  // unnamed namespace


TraceField* TraceField::create() {
    unique_ptr<TraceField> Ret(new(std::nothrow) TraceField());

    if (Ret && Ret->init())
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    return Ret.release();
}

TraceField* TraceField::createWithCustomCursor(const std::string &filename)
{
    unique_ptr<TraceField> Ret(new(std::nothrow) TraceField());
    Ret->cursorFilename = filename;
    
    if (Ret && Ret->init()) {
        Ret->autorelease();
    } else
        Ret.reset(nullptr);
    return Ret.release();
}

TraceField::TraceField()
    : DefaultCursorPickDistance(120.f)
    , CursorPickDistance(DefaultCursorPickDistance)
    , cursorFilename("")
{
    cursorScaleNormal = 1.75;
    cursorScalePicked = 2.1;
    
}

TraceField::~TraceField() {
    clearTraceSound();
}

bool TraceField::init() {
    if (!Super::init()) { return false; }
    
    TraceFieldDepot Depot;
    Depot.preloadSoundEffects();


    EnableDefaultBackground.OnValueUpdate = [this](bool&) {
        UseTightBoundingBox.update(EnableDefaultBackground());
    };
    EnableDefaultBackground.update(true);
    ClippingEnabled.update(true);

    TheStyle.update(([&] {
        // NB(xenosoz, 2016): The default style.
        using LT = TraceLineStyle::Type;
        TraceFieldStyle Style;

        // const float DalraeFontThickness = 144.f;
        // const float TodoSchoolFontThickness = 168.f;
        //const float TodoSchoolV2FontThickness = 144.f;
        const float TSAlphabetThickness = 144.f;

        const float Thickness = TSAlphabetThickness;

        Style.ScrollLetterByLetter = false;
        Style.MainLineThickness = Thickness;
        Style.LineStyles.emplace_back(LT::PassiveGuide, Thickness, Color4B(125, 63, 34, 255));
        Style.LineStyles.emplace_back(LT::PassiveStroke, Thickness, Color4B(247, 244, 242, 255));
        Style.LineStyles.emplace_back(LT::ActiveGuide, Thickness, Color4B(161, 90, 50, 255));
        Style.LineStyles.emplace_back(LT::ActiveStroke, Thickness, Color4B(247, 244, 242, 255));

        return Style;
    })());

    clear();
    return true;
}

void TraceField::setContentSize(const Size& ContentSize) {
    Super::setContentSize(ContentSize);
    
    refreshChildNodes();
}

void TraceField::clear() {
    clearTraceSound();

    Placeholder.update("");
    Text.update("");
    PassedIndex.update(TraceIndex());

    CursorPickDistance.update(DefaultCursorPickDistance);
    CursorPicked.update(false);
    
    using std::placeholders::_1;
    Enabled.OnValueUpdate = [this](bool&) {};
    Enabled.update(true);

    Placeholder.OnValueUpdate = bind(&TraceField::handlePlaceholderValueUpdate, this, _1);

    refreshChildNodes();
}

bool TraceField::traceWorkFinished() {
    bool Ret = (PassedIndex() == TraceIndex::end());
    return Ret;
}

void TraceField::onEnter() {
    Super::onEnter();

    // Install event listener.
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(TraceField::handleTouchBegan, this);
    listener->onTouchMoved = TouchEventRepeater(CC_CALLBACK_2(TraceField::handleTouchMoved, this), 5);
    listener->onTouchEnded = CC_CALLBACK_2(TraceField::handleTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(TraceField::handleTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();
}

void TraceField::onExit() {
    Super::onExit();
    this->unscheduleUpdate();

    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void TraceField::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);

    updateCursorPoint(DeltaSeconds);
}

void TraceField::handlePlaceholderValueUpdate(string&) {
    refreshChildNodes();
}

bool TraceField::handleTouchBegan(Touch* T, Event* E) {
    bool ShouldCatchEvent = handleActiveTouchEvent(T, E);
    if (!ShouldCatchEvent) { return false; }
    
    auto Guard = ScopeGuard([this] { retain(); },
                            [this] { release(); });

    if (OnBeginEditing)
        OnBeginEditing(this);

    return true;
}

void TraceField::handleTouchMoved(Touch* T, Event* E) {
    bool ShouldCatchEvent = handleActiveTouchEvent(T, E);
    if (!ShouldCatchEvent) { return; }

    auto Guard = ScopeGuard([this] { retain(); },
                            [this] { release(); });

    if (OnEditing)
        OnEditing(this);
}

void TraceField::handleTouchEnded(Touch* T, Event* E) {
    handlePassiveTouchEvent(T, E);
}

void TraceField::handleTouchCancelled(Touch* T, Event* E) {
    handlePassiveTouchEvent(T, E);
}

// NB(xenosoz, 2016): handleActiveTouchEvent: (...) -> CatchThisEvent.
bool TraceField::handleActiveTouchEvent(Touch* T, Event* E) {
    const bool CATCH_EVENT = true;
    const bool DONT_CATCH_EVENT = false;
    
    if (!Enabled())
        return DONT_CATCH_EVENT;

    // We don't catch any touch events from outside.
    Point PointInParentSpace = getParent()->convertToNodeSpace(T->getLocation());
    if (!getBoundingBox().containsPoint(PointInParentSpace))
        return DONT_CATCH_EVENT;

    if (!(PassedIndex().WorldIndex < 1 && PassedIndex().GlyphIndex < GlyphNodes.size())) {
        return DONT_CATCH_EVENT;
    }

    TraceGlyphNode* GlyphNode = GlyphNodes[PassedIndex().GlyphIndex].get();

    Point TouchPointInGlyph = GlyphNode->convertToNodeSpace(T->getLocation());
    Point TouchPointInField = this->convertToNodeSpace(T->getLocation());
    Point CursorPointInGlyph = CursorPointInGlyphNode();
    Point CursorPointInField = this->convertToNodeSpace(GlyphNode->convertToWorldSpace(CursorPointInGlyph));

    // NB(xenosoz, 2016): CursorPicked = CursorPicked || CursorPickedInThisEvent
    float CursorDist = TouchPointInGlyph.distance(CursorPointInGlyph);
    if (CursorDist < CursorPickDistance()) {
        CursorPicked.update(true);
    }

    if (CursorPicked()) {
        TraceLocator Locator;
        TraceIndex BestIndex = Locator.bestIndexByFinger(PassedIndex(), TheTraceString(), TouchPointInGlyph);
        bool IsBadFinger = Locator.isBadFinger(PassedIndex(), TheTraceString(), TouchPointInGlyph);

        if (PassedIndex() != BestIndex) {
            // NB(xenosoz, 2016): Play sound if the cursor advanced.
            const float SoundDuration = .05f;
            playTraceSound(SoundDuration);
        }

        if (BestIndex != TraceIndex::end()) {
            // NB(xenosoz, 2016): Update index.
            TracePoint BestPoint = BestIndex.pointFor(TheTraceString());
            PassedIndex.update(BestIndex);

            CursorPointInGlyphNode.update(BestPoint.Position);
            CursorVelocity.update(BestPoint.Velocity);
        }
        else if (IsBadFinger) {
            CursorPicked.update(false);

            // XXX: Animation required here.
        }
    }

    return CATCH_EVENT;
}

void TraceField::handlePassiveTouchEvent(Touch* touch, Event* event) {
    CursorPicked.update(false);

    TraceLocator Locator;
    if (Locator.isItGoodDayToAdvanceStroke(PassedIndex(), TheTraceString())) {
        TraceIndex NewIndex = PassedIndex().indexForStrokeEnd(TheTraceString());
        PassedIndex.update(NewIndex);

        if (NewIndex != TraceIndex::end()) {
            TracePoint NewPoint = NewIndex.pointFor(TheTraceString());
            CursorPointInGlyphNode.update(NewPoint.Position);
            CursorVelocity.update(NewPoint.Velocity);
        }
        else {
            // NB(xenosoz, 2016): We finished all tracing works here.
        }
    }

    clearTraceSound();

    auto Guard = ScopeGuard([this] { retain(); },
                            [this] { release(); });

    if (OnEndEditing)
        OnEndEditing(this);

    if (OnTraceWorkDidEnd && traceWorkFinished())
        OnTraceWorkDidEnd(this);
}

void TraceField::refreshChildNodes() {
    int ZOrder = 0;
    TraceFieldDepot Depot;

    if (true) {
        if (Background)
            Background->removeFromParent();

        if (EnableDefaultBackground()) {
            Background = ([&] {
                Sprite* It = Depot.createBackgroundSprite(getContentSize());
                It->setLocalZOrder(ZOrder++);
                addChild(It);
                return It;
            }());
        }
        else {
            Background.reset();
        }
    }

    if (true) {
        Rect PointBound = ([&] {
            Size S = getContentSize();
            Size Padding;

            if (EnableDefaultBackground()) {
                // NB(xenosoz, 2016): width 1218 = 188 + 842 + 188,
                //   height 1616 = 318 + 980 + 318.

                //S = Size(842.f, 980.f);
                S = Size(1218.f, 980.f);

                Padding.width = TheStyle().MainLineThickness;
                Padding.height = TheStyle().MainLineThickness;
            }
            else {
                // NB(xenosoz, 2016): I've got the request to make the font
                //   as large as possible. It seems that the height is not a deal breaker.
                //Padding.width = Depot.cursorMaxRadius() * 2;
                //Padding.height = Depot.cursorMaxRadius() * 2;
            }

            return Rect(Padding.width/2.f, Padding.height/2.f,
                        S.width - Padding.width, S.height - Padding.height);
        })();


        // NB(xenosoz, 2016): If we enable background, we should force alignment by hand.
        bool UseTightBB = EnableDefaultBackground();

        TraceModelFactory TMF;
        TraceString S = TMF.stringWithString(Placeholder());
        
        if (TheStyle().ScrollLetterByLetter) {
            // NB(xenososz, 2016): Scroll enabled
            //   -> Fit vertically.
            S = TMF.resizeStringVertically(S, PointBound, UseTightBB);
        }
        else {
            // NB(xenosoz, 2016): Scroll disabled
            //   -> Fit horizontally and vertically in the box.
            S = TMF.resizeString(S, PointBound, UseTightBB);
        }
        TheTraceString.update(S);
        REPR(S.roughBoundingBox());
        REPR(S.tightBoundingBox());
        
        CursorPicked.update(false);
        PassedIndex.update([&] {
            TraceIndex TI;
            TI.sanitize(TheTraceString());
            return TI;
        }());
        if (PassedIndex() != TraceIndex::end()) {
            TracePoint TP = PassedIndex().pointFor(TheTraceString());
            CursorPointInGlyphNode.update(TP.Position);
            CursorVelocity.update(TP.Velocity);
        }
    }

    if (true) {
        if (Scroll)
            Scroll->removeFromParent();

        Scroll = ([&](){
            TraceScrollView *It = TraceScrollView::create();
            Size CS = getContentSize();

            It->setContentSize(CS);
            It->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            It->setPosition(CS / 2.f);

            It->TheTraceString.follow(TheTraceString);
            It->PassedIndex.follow(PassedIndex);
            It->TheStyle.follow(TheStyle);
            It->ClippingEnabled.follow(ClippingEnabled);
            It->UseTightBoundingBox.follow(UseTightBoundingBox);

            It->setLocalZOrder(ZOrder++);
            addChild(It);
            return It;
        }());
    }

    if (true) {
        // NB(xenosoz, 2016): Reference info for graphic assets (by experiment):
        //   Dalrae = {scale: 3.f, glyphWidth: 950.f, lineThickness: 48.f}
        //   Custom = {scale: 2.f, glyphWidth: 850.f, lineThickness: 60.f}

        for (RefPtr<TraceGlyphNode> GlyphNode : GlyphNodes) {
            if (GlyphNode)
                GlyphNode->removeFromParent();
        }
        GlyphNodes.clear();

        for (size_t I = 0, E = TheTraceString().size(); I < E; ++I) {
            Size CS = getContentSize();
            const TraceGlyph& Glyph = TheTraceString()[I];
            Point Offset = TheTraceString().offsetForGlyphAt(I);

            TraceGlyphNode* It = TraceGlyphNode::create();
            
            It->setContentSize(Glyph.roughBoundingBox().size);
            It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            It->setPosition(Offset);
            It->setLocalZOrder(ZOrder);

            It->TheTraceString.follow(TheTraceString);
            It->BaseIndex.update(TraceIndex(I, 0, 0));
            It->PassedIndex.follow(PassedIndex);
            It->TheStyle.follow(TheStyle);

            Scroll->addChild(It);
            GlyphNodes.push_back(It);
        }
    }
    ZOrder += 1;

    if (true) {
        if (Cursor)
            Cursor->removeFromParent();

        Cursor = ([&] {
            TraceCursor *It;
            
            if (cursorFilename=="") {
                It = TraceCursor::create();
            } else {
                It = TraceCursor::createWithFilename(cursorFilename);
            }
            
            It->setLocalZOrder(ZOrder++);
            // NB(xenosoz, 2016): cursor will be a child of a TraceGlyphNode, not TraceField.
            //   It is not necessary to "addChild(It);" here.
            It->Enabled.follow(Enabled);
            It->CursorPointInGlyphNode.follow(CursorPointInGlyphNode);
            It->CursorVelocity.follow(CursorVelocity);
            It->CursorPicked.follow(CursorPicked);
            It->NormalScale.update(cursorScaleNormal);
            It->PickedScale.update(cursorScalePicked);
            return It;
        }());
    }
}

void TraceField::updateCursorPoint(float DeltaSeconds) {
    // NB(xenosoz): TraceCursor doesn't know the existance of TraceGlyphNode.
    //   Conversion should be happen here.
    if (PassedIndex().WorldIndex < 1 && PassedIndex().GlyphIndex < GlyphNodes.size()) {
        TraceGlyphNode *GlyphNode = GlyphNodes[PassedIndex().GlyphIndex].get();

        if (Cursor) {
            Cursor->removeFromParent();
            GlyphNode->addChild(Cursor);
        }
    }
}

void TraceField::clearTraceSound() {
    if (!TraceSound) { return; }
    
    if (showDebugLog()) {
        CCLOG("Turn off tracing sound in TraceField::clearTraceSound");
    }

    TraceSound.value().stop();
    TraceSound.reset();
}

void TraceField::playTraceSound(float Duration) {
    const string ClearSoundKey = "TraceField::playTraceSound::clearSound";

    // NB(xenosoz, 2016): Update lifetime of the sound.
    unschedule(ClearSoundKey);
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For clearing trace sound
        clearTraceSound();
    }, Duration, ClearSoundKey);


    // NB(xenosoz, 2016): Playing? -> Don't bother restarting the sound.
    if (TraceSound) { return; }

    if (showDebugLog()) {
        CCLOG("Trun on tracing sound in TraceField::playTraceSound");
    }

    SoundEffect SE = TraceFieldDepot().soundForTrace();
    SE.playLoop();
    TraceSound.reset(SE);
}

END_NS_TRACEFIELD
