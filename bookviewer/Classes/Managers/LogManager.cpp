//
//  LogManager.cpp
//  enumaXprize
//
//  Created by Seokkwon Lee on 11/1/16.
//
//

#include "cocos2d.h"
#include "LogManager.hpp"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

static LogManager *s_SharedLogManager = nullptr;
LogManager* LogManager::getInstance()
{
    if (!s_SharedLogManager)
    {
        s_SharedLogManager = new (std::nothrow) LogManager();
        CCASSERT(s_SharedLogManager, "FATAL: Not enough memory");
        s_SharedLogManager->init();
    }
    
    return s_SharedLogManager;
}

bool LogManager::init(void)
{
    return true;
}

bool LogManager::logEvent(std::string eventValue)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // get Documents directory path
    JniMethodInfo t;
    std::string eventString = eventValue;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "logEvent", "(Ljava/lang/String;)V"))
    {
        jstring jEventString = t.env->NewStringUTF(eventString.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jEventString);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jEventString);
        
        return true;
    }

#endif
    
    return false;
}

bool LogManager::logEventJson(std::string eventName, Json::Value jsonValue)
{
    jsonValue["action"] = eventName;
    
    Json::FastWriter fastWriter;
    std::string eventValue = fastWriter.write(jsonValue);
    
    return logEvent(eventValue);
}

bool LogManager::logEvent(std::string category, std::string action, std::string label, double value)
{
    Json::Value v;
    v["category"] = category;
    v["action"] = action;
    v["label"] = label;
    v["value"] = value;
    
    Json::FastWriter fastWriter;
    std::string eventValue = fastWriter.write(v);
    
    return logEvent(eventValue);
    
}

bool LogManager::tagScreen(std::string screenName)
{
    Json::Value v;
    v["category"] = screenName;
    v["action"] = "tagScreen";
    
    Json::FastWriter fastWriter;
    std::string eventValue = fastWriter.write(v);
    
    return logEvent(eventValue);
    
}



