//
//  AdultVoice_enUS.h -- Light voice of an adult male
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Common/Basic/SoundEffect.h>
#include <Common/Basic/TodoSchoolNS.h>


BEGIN_NS_TODOSCHOOL

class AdultVoice_enUS {
public:
    // NB(xenosoz, 2016): Cardinal numbers from 0 to 100
    SoundEffect cardinalNumber(int N) const;

    SoundEffect equals() const;
    SoundEffect minus() const;
    SoundEffect plus() const;
    SoundEffect times() const;
};

END_NS_TODOSCHOOL