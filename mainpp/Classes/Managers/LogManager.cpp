//
//  LogManager.cpp
//  enumaXprize
//
//  Created by Seokkwon Lee on 11/1/16.
//
//

#include "cocos2d.h"
#include "LogManager.hpp"
#include "platform/android/jni/JniHelper.h"

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
    _installID = UserDefault::getInstance()->getStringForKey("installID");
    if(_installID==""){
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "getRandomUUID", "()Ljava/lang/String;"))
        {
            jstring jInstallID = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
            _installID = JniHelper::jstring2string(jInstallID);
            UserDefault::getInstance()->setStringForKey("installID", _installID);
            UserDefault::getInstance()->flush();
        }
    }
    return true;
}

bool LogManager::logEvent(std::string eventName, std::string eventValue)
{
    // get Documents directory path
    JniMethodInfo t;
    std::string eventString = eventName.append(":").append(eventValue);
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "logEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jInstallId = t.env->NewStringUTF(_installID.c_str());
        jstring jEventString = t.env->NewStringUTF(eventString.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jInstallId, jEventString);
        t.env->DeleteLocalRef(t.classID);
        t.env->DeleteLocalRef(jInstallId);
        t.env->DeleteLocalRef(jEventString);
        
        return true;
    }
    
    return false;
}

bool LogManager::logEventJson(std::string eventName, Json::Value jsonValue)
{
    Json::FastWriter fastWriter;
    std::string eventValue = fastWriter.write(jsonValue);
    
    return logEvent(eventName, eventValue);
}
