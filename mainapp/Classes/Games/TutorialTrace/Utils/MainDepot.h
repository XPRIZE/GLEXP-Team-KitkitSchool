//
//  MainDepot.h -- Basic resource management for TutorialTrace
//  TodoSchool on Dec 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "MainNS.h"


BEGIN_NS_TUTORIALTRACE

class MainDepot {
public:
    string assetPrefix() const;
    string defaultFont() const;
    
    Size windowSize() const;
    Size gameSize() const;
    Size cardSize() const;
    
    float scaleToCover(Size FixedSize, Size SubjectSize) const;
    float scaleToBeContained(Size FixedSize, Size SubjectSize) const;
    float scaleToCoverWindow(Size SubjectSize) const;
    float scaleToBeContainedInWindow(Size SubjectSize) const;
};

END_NS_TUTORIALTRACE


