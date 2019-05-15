//
// Created by Taihyun Hwang on 16-Mar-2018.
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "VoiceMoldManager.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "cocos2d.h"
#include <string>
#include <limits>

using namespace cocos2d;
using namespace std;

std::string VoiceMoldManager::getDefaultLocale(std::string locale)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticGetDefaultLocale",
                                       "()Ljava/lang/String;"))
    {
        jstring jLocale = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        string locale = JniHelper::jstring2string(jLocale);

        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jLocale);
        
        return locale;
    }
    
    return "";
}

void VoiceMoldManager::setDefaultLocale(std::string locale)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticSetDefaultLocale",
                                       "(Ljava/lang/String;)V"))
    {
        jstring jLocale = t.env->NewStringUTF(locale.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jLocale);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jLocale);
    }
}

void VoiceMoldManager::secureVoiceData()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticSecureVoiceData", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void VoiceMoldManager::secureVoiceData(std::string locale)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticSecureVoiceData", "(Ljava/lang/String;)V"))
    {
        jstring jLocale = t.env->NewStringUTF(locale.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jLocale);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jLocale);
    }
}

void VoiceMoldManager::warmup()
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticWarmup", "()V"))
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void VoiceMoldManager::warmup(std::string locale)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticWarmup", "(Ljava/lang/String;)V"))
    {
        jstring jLocale = t.env->NewStringUTF(locale.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jLocale);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jLocale);
    }
}

void VoiceMoldManager::speak(std::string text)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticSpeak", "(Ljava/lang/String;)V"))
    {
        jstring jText = t.env->NewStringUTF(text.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jText);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jText);
    }
}

void VoiceMoldManager::speak(std::string text, std::string locale)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticSpeak", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jText = t.env->NewStringUTF(text.c_str());
        jstring jLocale = t.env->NewStringUTF(locale.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jText, jLocale);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jText);
        t.env->DeleteLocalRef(jLocale);
    }
}

float VoiceMoldManager::guessSpeakDuration(std::string text)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticGuessSpeakDuration", "(Ljava/lang/String;)F"))
    {
        jstring jText = t.env->NewStringUTF(text.c_str());
        float rv = t.env->CallStaticFloatMethod(t.classID, t.methodID, jText);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jText);
        
        return rv;
    }
    
    return std::numeric_limits<float>::quiet_NaN();
}

float VoiceMoldManager::guessSpeakDuration(std::string text, std::string locale)
{
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t,
                                       "org/cocos2dx/cpp/VoiceMoldManager",
                                       "staticGuessSpeakDuration", "(Ljava/lang/String;Ljava/lang/String;)F"))
    {
        jstring jText = t.env->NewStringUTF(text.c_str());
        jstring jLocale = t.env->NewStringUTF(locale.c_str());
        float rv = t.env->CallStaticFloatMethod(t.classID, t.methodID, jText, jLocale);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jText);
        t.env->DeleteLocalRef(jLocale);
        
        return rv;
    }
    
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
