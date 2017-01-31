//
//  LogManager.hpp
//  enumaXprize
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
    bool logEvent(std::string eventName, std::string eventValue);
    bool logEventJson(std::string eventName, Json::Value json);

private:
    std::string _installID;
};

#endif /* LogManager_hpp */
