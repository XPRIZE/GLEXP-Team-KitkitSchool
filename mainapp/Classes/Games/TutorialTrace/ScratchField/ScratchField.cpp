//
//  ScratchField.cpp on Dec 27, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//
//  Based on the great work from: todomathandroid_kh (Locomotive Labs)
//


#include "ScratchField.h"
#include <Common/Controls/TraceField/LiveModels/TraceIndex.h>


#include <Common/Controls/TraceField/Utils/TraceFieldDepot.h>
#include <Common/Controls/TraceField/Utils/TraceLocator.h>
#include <Common/Controls/TraceField/Utils/TraceModelFactory.h>

#include "Common/Basic/DeviceSpec.h"
#include "Common/Basic/ScopeGuard.h"
#include "Common/Basic/TouchEventRepeater.h"
#include <cocos/cocos2d.h>

#include "Common/Repr/AllRepr.h"


BEGIN_NS_TUTORIALTRACE

using namespace tracefield;

namespace {
    bool showDebugLog() { return true; }
}  // unnamed namespace


ScratchField* ScratchField::create() {
    unique_ptr<ScratchField> Ret(new(std::nothrow) ScratchField());
    
    if (Ret && Ret->init())
        Ret->autorelease();
    else
        Ret.reset(nullptr);
    return Ret.release();
}

ScratchField* ScratchField::createWithCustomCursor(const std::string &filename)
{
    unique_ptr<ScratchField> Ret(new(std::nothrow) ScratchField());
    Ret->cursorFilename = filename;
    
    if (Ret && Ret->init()) {
        Ret->autorelease();
    } else
        Ret.reset(nullptr);
    return Ret.release();
}

ScratchField::ScratchField()
: DefaultCursorPickDistance(120.f)
, CursorPickDistance(DefaultCursorPickDistance)
, cursorFilename("TutorialTrace/ScratchField/ScratchField_Cursor.png")
{
#if false
    cursorScaleNormal = 1.75;
    cursorScalePicked = 2.1;
#endif
    cursorScaleNormal = 1.10 * (118.f / 130.f);
    cursorScalePicked = 1.25 * (118.f / 130.f);
}

ScratchField::~ScratchField() {
    clearTraceSound();
}

bool ScratchField::init() {
    if (!Super::init()) { return false; }
    
    TraceFieldDepot Depot;
    Depot.preloadSoundEffects();

    
    EnableDefaultBackground.OnValueUpdate = [this](bool&) {
        UseTightBoundingBox.update(EnableDefaultBackground());
    };
    EnableDefaultBackground.update(true);
    ClippingEnabled.update(true);
    
    
    PassedIndex.OnValueUpdate = [this](TraceIndex&) {
        auto Guard = NodeScopeGuard(this);
        handlePassedIndexValueUpdate();
    };
    
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

    TraceSound = TraceFieldDepot().soundForTrace();
    TraceSoundPlaying = false;
    
    clear();
    return true;
}

void ScratchField::setContentSize(const Size& ContentSize) {
    Super::setContentSize(ContentSize);
    
    refreshChildNodes();
}

void ScratchField::clear() {
    clearTraceSound();
    
    StrokeName.update("");
    Text.update("");
    PassedIndex.update(TraceIndex());
    
    CursorPickDistance.update(DefaultCursorPickDistance);
    CursorPicked.update(false);
    
    using std::placeholders::_1;
    Enabled.OnValueUpdate = [this](bool&) {};
    Enabled.update(true);
    
    StrokeName.OnValueUpdate = [this](string&) {
        refreshChildNodes();
    };
    
    refreshChildNodes();
}

void ScratchField::appendStroke(Point P, const string& Top, const string& Bot) {
    size_t StrokeID = TheScratchStrokes.size();
    
    int LZPast = (-200 + (int)StrokeID);
    int LZPresent = (-100 + (int)StrokeID);
    int LZFuture = (-300 + (int)StrokeID);

    auto It = ScratchStroke::create(Top, Bot, LZPast, LZPresent, LZFuture);
    It->TheTraceString.follow(TheTraceString);
    It->BaseIndex.update(TraceIndex(StrokeID, 0));
    It->PassedIndex.follow(PassedIndex);

    It->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    It->setPosition(P);
    It->setLocalZOrder(LZFuture);

    addChild(It);
    TheScratchStrokes.push_back(It);
}

bool ScratchField::traceWorkFinished() {
    bool Ret = (PassedIndex() == TraceIndex::end());
    return Ret;
}

void ScratchField::onEnter() {
    Super::onEnter();
    
    // Install event listener.
    LiveTouchEventCount = 0;
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* T, Event* E) -> bool {
        TraceIndex Old = PassedIndex();
        bool Ret = handleTouchBegan(T, E);
        TraceIndex New = PassedIndex();
        
        drawScratchPath(Old, New);
        return Ret;
    };
    listener->onTouchMoved = [this](Touch* T, Event* E) {
        TraceIndex Old = PassedIndex();
        TouchEventRepeater(CC_CALLBACK_2(ScratchField::handleTouchMoved, this), 5)(T, E);
        TraceIndex New = PassedIndex();
        
        drawScratchPath(Old, New);
    };
    listener->onTouchEnded = CC_CALLBACK_2(ScratchField::handleTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(ScratchField::handleTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->scheduleUpdate();
}

void ScratchField::onExit() {
    Super::onExit();
    this->unscheduleUpdate();
    
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}

void ScratchField::update(float DeltaSeconds) {
    Super::update(DeltaSeconds);
    
    updateCursorPoint(DeltaSeconds);
}

bool ScratchField::handleTouchBegan(Touch* T, Event* E) {
    if (showDebugLog()) {
        CCLOG("ScratchField::handleTouchBegan: Live(%d)", LiveTouchEventCount);
    }

    bool ShouldCatchEvent = handleActiveTouchEvent(T, E);
    if (!ShouldCatchEvent) { return false; }
    
    auto Guard = ScopeGuard([this] { retain(); },
                            [this] { release(); });

    if (LiveTouchEventCount <= 0) {
        // NB(xenosoz, 2017): Fire event only for the first finger.
        if (OnBeginEditing)
            OnBeginEditing(this);
    }
    LiveTouchEventCount += 1;

    return true;
}

void ScratchField::handleTouchMoved(Touch* T, Event* E) {
    if (showDebugLog()) {
        //CCLOG("ScratchField::handleTouchMoved: Live(%d)", LiveTouchEventCount);
    }
    
    bool ShouldCatchEvent = handleActiveTouchEvent(T, E);
    if (!ShouldCatchEvent) { return; }
    
    auto Guard = ScopeGuard([this] { retain(); },
                            [this] { release(); });
    
    if (OnEditing)
        OnEditing(this);
}

void ScratchField::handleTouchEnded(Touch* T, Event* E) {
    LiveTouchEventCount -= 1;

    if (showDebugLog()) {
        CCLOG("ScratchField::handleTouchEnded: Live(%d)", LiveTouchEventCount);
    }

    handlePassiveTouchEvent(T, E);
}

void ScratchField::handleTouchCancelled(Touch* T, Event* E) {
    LiveTouchEventCount -= 1;
    
    if (showDebugLog()) {
        CCLOG("ScratchField::handleTouchCancelled: Live(%d)", LiveTouchEventCount);
    }

    handlePassiveTouchEvent(T, E);
}

// NB(xenosoz, 2016): handleActiveTouchEvent: (...) -> CatchThisEvent.
bool ScratchField::handleActiveTouchEvent(Touch* T, Event* E) {
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

        if (BestIndex != TraceIndex::end())
        {
            // NB(xenosoz, 2016): Update index.
            // NB(xenosoz, 2016): drawScratchPath is moved to onTouchMoved for optimization.
            PassedIndex.update(BestIndex);
            
            TracePoint BestPoint = BestIndex.pointFor(TheTraceString());
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

void ScratchField::handlePassiveTouchEvent(Touch* touch, Event* event) {
    if (LiveTouchEventCount > 0) {
        // NB(xenosoz, 2017): We wait for all fingers to be released.
        return;
    }

    CursorPicked.update(false);
    
    TraceLocator Locator;
    if (Locator.isItGoodDayToAdvanceStroke(PassedIndex(), TheTraceString())) {
        TraceIndex NewIndex = PassedIndex().indexForStrokeEnd(TheTraceString());
        drawScratchPath(PassedIndex(), NewIndex);
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
    
    if (traceWorkFinished()) {
        if (OnTraceWorkDidEnd)
            OnTraceWorkDidEnd(this);
    }
}

void ScratchField::refreshChildNodes() {
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
        TraceString S = [&] {
            TraceString String;

            Json::Value JsonValue;
            string JsonStr = FileUtils::getInstance()->getStringFromFile(StrokeName());
            Json::Reader JsonReader;
            if (!JsonReader.parse(JsonStr, JsonValue)) {
                CCLOGERROR("JSON reader parse error: %s", StrokeName().c_str());
                JsonValue.clear();
            }
            else {
                CCLOG("JSON reader parse done: %s", StrokeName().c_str());
            }
            
            TraceKnotList KnotList(JsonValue);
            auto CS = getContentSize();
            auto G = TMF.glyphWithKnotList(KnotList, Rect(0, 0, CS.width, CS.height));
            String.appendGlyph(G);
            String.refreshBoundingBox();
            return String;
        }();
        
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
            // NB(xenosoz, 2016): cursor will be a child of a TraceGlyphNode, not ScratchField.
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

void ScratchField::updateCursorPoint(float DeltaSeconds) {
    // NB(xenosoz): TraceCursor doesn't know the existance of TraceGlyphNode.
    //   Conversion should be happen here.
    if (PassedIndex().WorldIndex < 1 && PassedIndex().GlyphIndex < GlyphNodes.size()) {
        TraceGlyphNode *GlyphNode = GlyphNodes[PassedIndex().GlyphIndex].get();
        
        if (Cursor && Cursor->getParent() != GlyphNode) {
            Cursor->removeFromParent();
            GlyphNode->addChild(Cursor);
        }
    }
}

void ScratchField::drawScratchPath(TraceIndex BeginIndex, TraceIndex EndIndex)
{
    if (PassedIndex().GlyphIndex >= GlyphNodes.size()) { return; }
    if (PassedIndex().GlyphIndex >= TheScratchStrokes.size()) { return; }

    TraceGlyphNode* GlyphNode = GlyphNodes[PassedIndex().GlyphIndex].get();
    ScratchStroke* ScratchNode = TheScratchStrokes[PassedIndex().StrokeIndex];
    
    // NB(xenosoz, 2016): Step 90, Radius 75 is fast but bad at intersections.
    //   Step 95, Radius 75 is just bad as we can feel the gap.
    //   Radius 35 is tolerable, Step 20 is too much, Step 1 is too slow.
    //   So I changed the routine to use line segments instead of points.
    
    // NB(xenosoz, 2016): Now Step 20 is good.
    size_t Step = 40;
    float Radius = 40.f;
    
    vector<Point> PS;
    auto appendPointForIndex = [&](TraceIndex& TI) {
        auto P = TI.pointFor(TheTraceString()).Position;
        auto WorldP = GlyphNode->convertToWorldSpace(P);
        auto LocalP = ScratchNode->convertToNodeSpace(WorldP);
        
        PS.push_back(LocalP);
    };
    
    TraceIndex From = max(BeginIndex, EndIndex.indexForStrokeBegin(TheTraceString()));
    for (TraceIndex I = From; I < EndIndex; I.increment(TheTraceString(), Step)) {
        appendPointForIndex(I);
    }

    if (EndIndex != TraceIndex::end())
        appendPointForIndex(EndIndex);

    // NB(xenosoz, 2016): Scratch the mask to reveal bottom images.
    if (!PS.empty())
        ScratchNode->scratchCircle(PS[0], Radius);
    
    if (PS.size() >= 2) {
        ScratchNode->scratchSegments(PS, Radius * 2);
        ScratchNode->scratchCircle(PS[PS.size() - 1], Radius);
    }
}

void ScratchField::handlePassedIndexValueUpdate() {
}

void ScratchField::clearTraceSound() {
    if (showDebugLog()) {
        CCLOG("Turn off tracing sound in ScratchField::clearTraceSound");
    }
    
    TraceSound.stop();
    TraceSoundPlaying = false;
}

void ScratchField::playTraceSound(float Duration) {
    const string ClearSoundKey = "ScratchField::playTraceSound::clearSound";
    
    // NB(xenosoz, 2016): Update lifetime of the sound.
    unschedule(ClearSoundKey);
    scheduleOnce([this](float) {  // NB(xenosoz, 2016): For clearing trace sound
        clearTraceSound();
    }, Duration, ClearSoundKey);
    
    
    // NB(xenosoz, 2016): Playing? -> Don't bother restarting the sound.
    if (TraceSoundPlaying) { return; }
    
    if (showDebugLog()) {
        CCLOG("Trun on tracing sound in ScratchField::playTraceSound");
    }
    
    TraceSound.playLoop();
    TraceSoundPlaying = true;
}

END_NS_TUTORIALTRACE
