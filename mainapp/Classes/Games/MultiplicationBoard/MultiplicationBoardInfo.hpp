//
//  MultiplicationBoardInfo.hpp
//  KitkitSchool
//
//  Created by harunom on 7/21/18.
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Common/Basic/KitkitGameInfo.hpp"

#include "MultiplicationBoardScene.hpp"

#include <string>
#include <vector>

using namespace std;



class MultiplicationBoardInfo : public KitkitGameInfo
{
public:
    virtual std::string getGameName() { return "MultiplicationBoard"; }
    virtual std::string getSceneName() { return "MultiplicationBoardScene"; }
    
    virtual Scene* createScene(std::string levelID) {
        return MultiplicationBoardScene::createScene(levelID);
    }
    
    
    virtual std::vector<std::string> enumerateLevelIDs();
    int getMaxLevel();
    
    
    
};

