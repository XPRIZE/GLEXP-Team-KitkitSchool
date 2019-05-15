//
//  LogManager.mm
//  KitkitSchool
//
//  Created by Seokkwon Lee on 11/1/16.
//
//

#import <Foundation/Foundation.h>
#import "LogManager.hpp"

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
    return NO;
}

bool LogManager::logEventJson(std::string eventName, Json::Value json)
{
    NSString *nsEventName = [NSString stringWithCString:eventName.c_str() encoding:NSUTF8StringEncoding];
    
    Json::FastWriter fastWriter;
    std::string eventValue = fastWriter.write(json);
    NSString *nsJson = [NSString stringWithCString:eventValue.c_str() encoding:NSUTF8StringEncoding];
    
    NSLog(@"logEventJson %@ %@", nsEventName, nsJson);
    return NO;
}

bool LogManager::logEvent(std::string category, std::string action, std::string label, double value)
{
    return NO;
}

bool LogManager::tagScreen(std::string screenName)
{
    return NO;
}
