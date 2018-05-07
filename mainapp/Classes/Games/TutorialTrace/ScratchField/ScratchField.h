//
//  ScratchField.h on Dec 27, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "ScratchStroke.h"
#include <Common/Controls/TraceField/ChildNodes/TraceScrollView.h>
#include <Common/Controls/TraceField/ChildNodes/TraceGlyphNode.h>
#include <Common/Controls/TraceField/ChildNodes/TraceCursor.h>
#include <Common/Controls/TraceField/LiveModels/TraceString.h>
#include <Common/Controls/TraceField/LiveModels/TraceIndex.h>
#include <Common/Controls/TraceField/StyleModels/TraceFieldStyle.h>

#include "../Utils/MainNS.h"


BEGIN_NS_TUTORIALTRACE

using tracefield::TraceScrollView;
using tracefield::TraceGlyphNode;
using tracefield::TraceCursor;
using tracefield::TraceString;
using tracefield::TraceIndex;
using tracefield::TraceFieldStyle;


//
// NB(xenosoz, 2016): Text : TextField = Trace : ScratchField
//

class ScratchField: public Node {
    typedef Node Super;
    
    // NB(xenosoz, 2016): We pick cursor within this distance.
    const float DefaultCursorPickDistance;
    
    // NB(xenosoz, 2016): Child nodes.
    RefPtr<Sprite> Background;
    RefPtr<TraceScrollView> Scroll;
    vector<RefPtr<TraceGlyphNode>> GlyphNodes;
    
    bool TraceSoundPlaying;
    SoundEffect TraceSound;
    string cursorFilename;
    
    
public:
    RefPtr<TraceCursor> Cursor;
    float cursorScaleNormal, cursorScalePicked;
    
    
    // NB(xenosoz, 2016): Setting StrokeName resets all relevant members of ScratchField.
    Chain<string> StrokeName;
    Chain<TraceString> TheTraceString;
    Chain<TraceIndex> PassedIndex;
    Chain<string> Text;
    
    Chain<bool> EnableDefaultBackground;
    Chain<bool> UseTightBoundingBox;
    Chain<TraceFieldStyle> TheStyle;
    
    Chain<Point> CursorPointInGlyphNode;
    Chain<Vec2> CursorVelocity;
    Chain<float> CursorPickDistance;
    Chain<bool> CursorPicked;
   
    vector<ScratchStroke*> TheScratchStrokes;
    
    // NB(xenosoz, 2016): The control responds to user event only when it's enabled.
    Chain<bool> Enabled;
    Chain<bool> ClippingEnabled;
    
    int LiveTouchEventCount;
    function<void(ScratchField*)> OnBeginEditing;
    function<void(ScratchField*)> OnEditing;
    function<void(ScratchField*)> OnEndEditing;
    function<void(ScratchField*)> OnTraceWorkDidEnd;
    
public:
    static ScratchField* create();
    static ScratchField* createWithCustomCursor(const std::string &filename);
    ScratchField();
    virtual ~ScratchField();
    
    bool init() override;
    void setContentSize(const Size& ContentSize) override;
    
    void clear();
    void appendStroke(Point P, const string& Top, const string& Bot);
    bool traceWorkFinished();
    
private:
    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;
    
    bool handleTouchBegan(Touch* touch, Event* event);
    void handleTouchMoved(Touch* touch, Event* event);
    void handleTouchEnded(Touch* touch, Event* event);
    void handleTouchCancelled(Touch* touch, Event* event);
    bool handleActiveTouchEvent(Touch* touch, Event* event);
    void handlePassiveTouchEvent(Touch* touch, Event* event);
    
private:
    void refreshChildNodes();
    void updateCursorPoint(float DeltaSeconds);
    void drawScratchPath(TraceIndex BeginIndex, TraceIndex EndIndex);
    void handlePassedIndexValueUpdate();
    
    // NB(xenosoz, 2016): Duration is in seconds.
    void playTraceSound(float Duration);
    void clearTraceSound();
};

END_NS_TUTORIALTRACE
