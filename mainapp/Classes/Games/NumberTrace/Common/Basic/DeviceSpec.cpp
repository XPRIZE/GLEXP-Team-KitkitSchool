//
//  DeviceSpec.cpp on Apr 22, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#include "DeviceSpec.h"
#include <cocos/cocos2d.h>

using namespace todoschool;
using namespace cocos2d;
using namespace std;

DeviceSpec DeviceSpec::forCurrentDevice() {
    DeviceSpec Spec;
    Spec.Name = "__CURRENT_DEVICE__";
    Spec.FullScreenResolution = Director::getInstance()->getOpenGLView()->getFrameSize();
    return Spec;
}

DeviceSpec DeviceSpec::forCurrentDesignResolution() {
    DeviceSpec Spec;
    Spec.Name = "__CURRENT_DESIGN_RESOLUTION__";
    Spec.FullScreenResolution = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    return Spec;
}

DeviceSpec DeviceSpec::Google_Nexus_7_2013() {
    DeviceSpec Spec;
    Spec.Name = "Nexus 7 (2013)";
    Spec.FullScreenResolution = Size(1920, 1200);
    return Spec;
}

DeviceSpec DeviceSpec::Google_Nexus_9()
{
    DeviceSpec Spec;
    Spec.Name = "Nexus 9";
    Spec.FullScreenResolution = Size(2048, 1536);
#if 0
    Spec.DistractedResolution = Size(2048, 1440);  // Size with on-screen buttons.
#endif
    return Spec;
}

DeviceSpec DeviceSpec::Google_Pixel_C()
{
    DeviceSpec Spec;
    Spec.Name = "Pixel C";
    Spec.FullScreenResolution = Size(2560, 1800);
    return Spec;
}