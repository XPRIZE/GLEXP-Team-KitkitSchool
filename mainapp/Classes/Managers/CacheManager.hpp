//
//  CacheManager.hpp
//  KitkitSchool-mobile
//
//  Created by JungJaehun on 16/05/2018.
//

#ifndef CacheManager_hpp
#define CacheManager_hpp

#include <stdio.h>

#include "Common/Controls/TouchEventLogger.h"

#include <string>
#include <vector>
#include <functional>

using namespace std;

class CacheManager {
    static CacheManager* _instance;
public:
    static CacheManager* getInstance();
    void init();
    
    void loadBirdCache(std::function<void()> callback);
    void loadCoopCache();
    void loadDailyCache();

private:
    string _lastFilePath;
};

#endif /* CacheManager_hpp */
