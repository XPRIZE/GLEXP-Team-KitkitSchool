//
//  DeviceSpec.h on Apr 22, 2016
//  TodoSchool
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//

#ifndef __TODOSCHOOL__DEVICESPEC_H__
#define __TODOSCHOOL__DEVICESPEC_H__

#include <string>
#include <cocos/math/CCGeometry.h>

namespace todoschool {

class DeviceSpec
{
public:
    std::string Name;
    cocos2d::Size FullScreenResolution;

public:
    static DeviceSpec forCurrentDevice();
    static DeviceSpec forCurrentDesignResolution();
    static DeviceSpec Google_Nexus_7_2013(); // 2013-07-26
    static DeviceSpec Google_Nexus_9();  // 2014-11-03
    static DeviceSpec Google_Pixel_C();  // 2015-12-08
};

} // end namespace todoschool
#endif // !defined(__TODOSCHOOL__DEVICESPEC_H__)
