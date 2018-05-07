//
//  CommonSound.cpp
//  KitkitSchool
//
//  Created by Gunho Lee on 12/22/17.
//

#include "CommonSound.hpp"


#include "Managers/LanguageManager.hpp"
#include "Utils/StringUtils.h"

USING_NS_CC;

SoundEffect CommonSound::soundForPhonic(const std::string& Phonic) const
{
    auto L = todoschool::toLower(Phonic);
    
    auto f = cocos2d::StringUtils::format("PhonicVoice/%s_sound.m4a", L.c_str());
    if (FileUtils::getInstance()->isFileExist(f)) {
        return SoundEffect(f);
    }
    
    f = cocos2d::StringUtils::format("LetterVoice/%s.m4a", L.c_str());
    if (FileUtils::getInstance()->isFileExist(f)) {
        return SoundEffect(f);
    }
    
    return SoundEffect();
    
}


SoundEffect CommonSound::soundForLetterName(const std::string& Letter) const {
    
    auto L = todoschool::toLower(Letter);
    auto f = cocos2d::StringUtils::format("LetterVoice/%s.m4a", L.c_str());
    
    if (FileUtils::getInstance()->isFileExist(f)) {
        return SoundEffect(f);
    }
    
    if (LanguageManager::getInstance()->isEnglish()) {
        f = cocos2d::StringUtils::format("LetterVoice/eng_%s.m4a", L.c_str());
        return SoundEffect(f);
    }
    
    return SoundEffect();
    
}

SoundEffect CommonSound::soundForWord(const std::string& Word) const {
    
    auto path = cocos2d::StringUtils::format("WordVoice/%s.m4a", Word.c_str());
    if (!cocos2d::FileUtils::getInstance()->isFileExist(path)) {
        path = cocos2d::StringUtils::format("WordVoice/%s.wav", Word.c_str());
    }
    return SoundEffect(path.c_str());
    
}




SoundEffect CommonSound::cardinalNumber(int N) const {
    if (0 <= N && N <= 100) {
        return SoundEffect(cocos2d::StringUtils::format("NumberVoice/D_%d.m4a", N));
    }
    return SoundEffect::emptyEffect();
}

SoundEffect CommonSound::equals() const {
    return SoundEffect("NumberVoice/Equals.m4a");
}

SoundEffect CommonSound::minus() const {
    return SoundEffect("NumberVoice/Minus.m4a");
}

SoundEffect CommonSound::plus() const {
    return SoundEffect("NumberVoice/Plus.m4a");
}

SoundEffect CommonSound::times() const {
    return SoundEffect("NumberVoice/Times.m4a");
}

