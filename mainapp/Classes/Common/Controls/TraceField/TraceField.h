//
//  TraceField.h on Apr 21, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "ChildNodes/TraceScrollView.h"
#include "ChildNodes/TraceGlyphNode.h"
#include "ChildNodes/TraceCursor.h"
#include "LiveModels/TraceString.h"
#include "LiveModels/TraceIndex.h"
#include "StyleModels/TraceFieldStyle.h"

#include "Utils/MainNS.h"


BEGIN_NS_TRACEFIELD

//
// NB(xenosoz, 2016): Text : TextField = Trace : TraceField
//

class TraceField: public Node {
    typedef Node Super;

    // NB(xenosoz, 2016): We pick cursor within this distance.
    const float DefaultCursorPickDistance;

    // NB(xenosoz, 2016): Child nodes.
    RefPtr<Sprite> Background;
    RefPtr<TraceScrollView> Scroll;
    vector<RefPtr<TraceGlyphNode>> GlyphNodes;

    
    Optional<SoundEffect> TraceSound;
    string cursorFilename;
    

public:
    RefPtr<TraceCursor> Cursor;
    float cursorScaleNormal, cursorScalePicked;
    
    
    // NB(xenosoz, 2016): Setting placeholder resets all relevant members of TraceField.
    Chain<string> Placeholder;
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

    // NB(xenosoz, 2016): The control responds to user event only when it's enabled.
    Chain<bool> Enabled;
    Chain<bool> ClippingEnabled;

    function<void(TraceField*)> OnBeginEditing;
    function<void(TraceField*)> OnEditing;
    function<void(TraceField*)> OnEndEditing;
    function<void(TraceField*)> OnTraceWorkDidEnd;

    bool mbTouched;
public:
    static TraceField* create();
    static TraceField* createWithCustomCursor(const std::string &filename);
    TraceField();
    virtual ~TraceField();

    bool init() override;
    void setContentSize(const Size& ContentSize) override;

    void clear();
    bool traceWorkFinished();

private:
    void onEnter() override;
    void onExit() override;
    void update(float DeltaSeconds) override;

    void handlePlaceholderValueUpdate(string&);
    bool handleTouchBegan(Touch* touch, Event* event);
    void handleTouchMoved(Touch* touch, Event* event);
    void handleTouchEnded(Touch* touch, Event* event);
    void handleTouchCancelled(Touch* touch, Event* event);
    bool handleActiveTouchEvent(Touch* touch, Event* event);
    void handlePassiveTouchEvent(Touch* touch, Event* event);

private:
    void refreshChildNodes();
    void updateCursorPoint(float DeltaSeconds);

    // NB(xenosoz, 2016): Duration is in seconds.
    void playTraceSound(float Duration);
    void clearTraceSound();
};

END_NS_TRACEFIELD
