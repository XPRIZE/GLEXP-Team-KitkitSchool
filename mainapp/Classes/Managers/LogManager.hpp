//
//  LogManager.hpp
//  KitkitSchool
//
//  Created by Seokkwon Lee on 11/1/16.
//
//

#ifndef LogManager_hpp
#define LogManager_hpp

#include <stdio.h>
#include "3rdParty/json/json.h"

class LogManager {
public:
    static LogManager *getInstance();
    LogManager(){};
    virtual bool init();
    virtual ~LogManager(){};
    
    bool logEventJson(std::string eventName, Json::Value json);
    bool logEvent(std::string category, std::string action, std::string label, double value);
    bool tagScreen(std::string screenName);
    
private:
    bool logEvent(std::string eventValue);
};

#endif /* LogManager_hpp */
