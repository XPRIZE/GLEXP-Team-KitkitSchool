//
//  Imma_swTZ.h -- Rich voice of a female adult
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

class Imma_swTZ {
public:
    // NB(xenosoz, 2016): Swahili letters. Letter.size() may be larger than 1. (ny, ng', ...)
    SoundEffect soundForLetterName(const std::string& Letter) const;
    
    SoundEffect soundForWord(const std::string& Word) const;
};

END_NS_TODOSCHOOL
