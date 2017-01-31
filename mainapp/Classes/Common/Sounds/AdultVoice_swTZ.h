//
//  AdultVoice_swTZ.h -- Rich voice of a female adult
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Common/Basic/SoundEffect.h>
#include <Common/Basic/TodoSchoolNS.h>


BEGIN_NS_TODOSCHOOL

class AdultVoice_swTZ {
public:
    // NB(xenosoz, 2016): Cardinal numbers from 1 to 30
    SoundEffect cardinalNumber(int N) const;
    
    SoundEffect equals() const;
    SoundEffect minus() const;
    SoundEffect plus() const;
};

END_NS_TODOSCHOOL