//
//  KitkitGameInfo.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 7/14/18.
//

#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include <vector>
#include "cocos2d.h"

USING_NS_CC;


class KitkitGameInfo {
protected:
    static std::map<std::string, KitkitGameInfo*> _infos;
    
public:
    static void registerGame(KitkitGameInfo* info) { _infos[info->getGameName()] = info; }
    static std::vector<KitkitGameInfo*> getRegisteredGames();
    
    static KitkitGameInfo* findGameInfo(std::string gameName) {
        auto ret = _infos.find(gameName);
        if (ret==_infos.end()) return nullptr;
        return ret->second;
    }
    
   
public:
    
    virtual std::string getGameName() = 0;
    virtual std::string getSceneName() = 0;
    

    virtual Scene* createScene(std::string levelID) = 0;
    
    virtual std::vector<std::string> enumerateLevelIDs() = 0;
    

    
    
    
    
};

