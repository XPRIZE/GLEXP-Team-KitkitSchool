//
//  TraceFieldStyle.h -- Custom style properties
//  TodoSchool on Jul 3, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "TraceLineStyle.h"
#include "../Utils/TraceFieldNS.h"


BEGIN_NS_TRACEFIELD

class TraceFieldStyle {
public:
    bool ScrollLetterByLetter;
    float MainLineThickness;
    TraceLineStyles LineStyles;
};

END_NS_TRACEFIELD
