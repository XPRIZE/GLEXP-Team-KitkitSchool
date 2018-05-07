//
//  MainDepot.cpp -- Basic resource management for DigitalQuiz
//  TodoSchool on Oct 31, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#include "MainDepot.h"
#include "Common/Basic/DeviceSpec.h"
#include <Managers/LanguageManager.hpp>


BEGIN_NS_DIGITALQUIZ

string MainDepot::assetPrefix() const {
    return "DigitalQuiz";
}

string MainDepot::defaultFont() const {
    return "fonts/TodoSchoolV2.ttf";
}

float MainDepot::defaultFontSize() const {
    return 90.f;
}

Color3B MainDepot::defaultFontColor() const {
    return Color3B(23, 163, 232);
}

Color3B MainDepot::highlightFontColor() const {
    return Color3B(252, 105, 134);
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

GLubyte MainDepot::opacityForDisabledButton() const {
    // NB(xenosoz, 2016): Set Ratio < 1.f to apply hide-and-show effect.
    float Ratio = 1.00f;

    return 255 * Ratio;
}

float MainDepot::fadeInDurationForQuizButton() const {
    return .2f;
}

END_NS_DIGITALQUIZ


