//
// Created by Taihyun Hwang on 26/02/2018.
//

#pragma once

#include <string>


class VoiceMoldManager
{
public:
    // NB(xenosoz, 2018): To copy these std::string prevent crash
    //   on some device. I'm very sad I don't know why, but it works.

    std::string getDefaultLocale(std::string locale);
    void setDefaultLocale(std::string locale);

    // NB(xenosoz, 2018): Issue an (android) intent to install voice data.
    void secureVoiceData();
    void secureVoiceData(std::string locale);

    void warmup();
    void warmup(std::string locale);

    void speak(std::string text);
    void speak(std::string text, std::string locale);

    float guessSpeakDuration(std::string text);
    float guessSpeakDuration(std::string text, std::string locale);

public:
    static VoiceMoldManager* shared();
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
private:
    std::string _internalLocaleForMock;
#endif
};

//EOF
