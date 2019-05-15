//
// Created by Taihyun Hwang on 16-Mar-2018.
//

#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

#include "VoiceMoldManager.h"
#include <string>
#include <limits>
#include <cocos/cocos2d.h>


std::string VoiceMoldManager::getDefaultLocale(std::string locale)
{
    return _internalLocaleForMock;
}

void VoiceMoldManager::setDefaultLocale(std::string locale)
{
    _internalLocaleForMock = locale;
}

void VoiceMoldManager::secureVoiceData()
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::secureVoiceData()");
}

void VoiceMoldManager::secureVoiceData(std::string locale)
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::secureVoiceData()");
}

void VoiceMoldManager::warmup()
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::warmup()");
}

void VoiceMoldManager::warmup(std::string locale)
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::warmup(...)");
}

void VoiceMoldManager::speak(std::string text)
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::speak(...)");
}

void VoiceMoldManager::speak(std::string text, std::string locale)
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::speak(...)");
}

float VoiceMoldManager::guessSpeakDuration(std::string text)
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::guessSpeakDuration(...)");
    return std::numeric_limits<float>::quiet_NaN();
}

float VoiceMoldManager::guessSpeakDuration(std::string text, std::string locale)
{
    CCLOG("Trying to use empty VoiceMoldManager: see VoiceMoldManager::guessSpeakDuration(...)");
    return std::numeric_limits<float>::quiet_NaN();
}

VoiceMoldManager* VoiceMoldManager::shared()
{
    static VoiceMoldManager* _shared = nullptr;
    if (!_shared) {
        _shared = new VoiceMoldManager();
    }
    return _shared;
}

#endif
//EOF
