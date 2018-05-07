//
//  MainDepot.cpp -- Basic resource management for TutorialTrace
//  TodoSchool on Dec 20, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>


BEGIN_NS_TUTORIALTRACE

string MainDepot::assetPrefix() const {
    return "TutorialTrace";
}

string MainDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

Size MainDepot::windowSize() const {
    return DeviceSpec::forCurrentDesignResolution().FullScreenResolution;
}

Size MainDepot::gameSize() const {
    return DeviceSpec::Google_Pixel_C().FullScreenResolution;
}

Size MainDepot::cardSize() const {
    return Size(218.f, 338.f);
}

float MainDepot::scaleToCover(Size FixedSize, Size SubjectSize) const {
    float Scale = max(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float MainDepot::scaleToBeContained(Size FixedSize, Size SubjectSize) const {
    float Scale = min(FixedSize.width / SubjectSize.width,
                      FixedSize.height / SubjectSize.height);
    return Scale;
}

float MainDepot::scaleToCoverWindow(Size SubjectSize) const {
    return scaleToCover(windowSize(), SubjectSize);
}

float MainDepot::scaleToBeContainedInWindow(Size SubjectSize) const {
    return scaleToBeContained(windowSize(), SubjectSize);
}

END_NS_TUTORIALTRACE
