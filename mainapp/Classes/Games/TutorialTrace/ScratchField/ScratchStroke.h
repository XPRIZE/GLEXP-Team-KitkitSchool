//
//  ScratchStroke.h on Dec 27, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "../Utils/TutorialTraceNS.h"
#include <Common/Controls/TraceField/LiveModels/TraceString.h>
#include <Common/Controls/TraceField/LiveModels/TraceIndex.h>


BEGIN_NS_TUTORIALTRACE

using tracefield::TraceString;
using tracefield::TraceIndex;


class ScratchStroke: public Node {
    typedef Node Super;
    
    Node* BotNode;
    DrawNode* MaskNode;
    Node* ClipNode;
    Node* TopNode;
    
public:
    Chain<TraceString> TheTraceString;
    Chain<TraceIndex> BaseIndex;
    Chain<TraceIndex> PassedIndex;
    
    int LocalZIndexForPast;
    int LocalZIndexForPresent;
    int LocalZIndexForFuture;

public:
    CREATE_FUNC(ScratchStroke);
    ScratchStroke();
    bool init() override;
    bool init(const string& Top, const string& Bot, int Past, int Present, int Future);
    
    void scratchCircle(Point P, float Radius);
    void scratchSegments(const vector<Point>& PS, float Radius);
};

END_NS_TUTORIALTRACE
