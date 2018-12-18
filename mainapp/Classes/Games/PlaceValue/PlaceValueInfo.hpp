//
//  PlaceValueInfo.hpp
//  KitkitSchool
//
//  Created by harunom on 8/31/18.
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Common/Basic/KitkitGameInfo.hpp"

#include "PlaceValueScene.hpp"

#include <string>
#include <vector>

using namespace std;



class PlaceValueInfo : public KitkitGameInfo
{
public:
    virtual std::string getGameName() { return "PlaceValue"; }
    virtual std::string getSceneName() { return "PlaceValueScene"; }
    
    virtual Scene* createScene(std::string levelID) {
        return PlaceValueScene::createScene(levelID);
    }
    
    
    virtual std::vector<std::string> enumerateLevelIDs();
    int getMaxLevel();
    
    
    
};

