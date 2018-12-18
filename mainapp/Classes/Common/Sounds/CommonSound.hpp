//
//  CommonSound.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/22/17.
//



#pragma once

#include "cocos2d.h"

#include <Common/Basic/SoundEffect.h>
#include <Common/Basic/TodoSchoolNS.h>
#include <string>



class CommonSound {
public:
    SoundEffect soundForPhonic(const std::string& Phonic) const;
    SoundEffect soundForLetterName(const std::string& Letter) const;
    SoundEffect soundForWord(const std::string& Word) const;
    
    SoundEffect cardinalNumber(int N) const;
    SoundEffect equals() const;
    SoundEffect minus() const;
    SoundEffect plus() const;
    SoundEffect times() const;
    SoundEffect aand() const;
    
    
};


