//
//  MainDepot.h -- Basic resource management for DigitalQuiz
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include "MainNS.h"


BEGIN_NS_DIGITALQUIZ

class MainDepot {
public:
    string assetPrefix() const;
    string defaultFont() const;

    float defaultFontSize() const;
    Color3B defaultFontColor() const;
    Color3B highlightFontColor() const;
    
    Size windowSize() const;
    Size gameSize() const;
    Size cardSize() const;
    
    float scaleToCover(Size FixedSize, Size SubjectSize) const;
    float scaleToBeContained(Size FixedSize, Size SubjectSize) const;
    float scaleToCoverWindow(Size SubjectSize) const;
    float scaleToBeContainedInWindow(Size SubjectSize) const;
    
    GLubyte opacityForDisabledButton() const;
    float fadeInDurationForQuizButton() const;
};

END_NS_DIGITALQUIZ


