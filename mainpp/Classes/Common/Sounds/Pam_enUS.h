//
//  Pam_enUS.h -- A voice of a female adult
//  TodoSchool on Aug 25, 2016
//
//  Copyright (c) 2016 Enuma, Inc. All rights reserved.
//  See LICENSE.md for more details.
//


#pragma once

#include <Common/Basic/SoundEffect.h>
#include <Common/Basic/TodoSchoolNS.h>
#include <string>


BEGIN_NS_TODOSCHOOL

class Pam_enUS {
public:
    // NB(xenosoz, 2016): English letter [a-z]. [A-Z] will be converted to [a-z] internally.
    SoundEffect soundForLetterName(const std::string& Letter) const;
    
    SoundEffect soundForWord(const std::string& Word) const;
};

END_NS_TODOSCHOOL
