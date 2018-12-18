//
//  WordKickerInfo.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/14/18.
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Common/Basic/KitkitGameInfo.hpp"

#include "WordKickerScene.hpp"

#include <string>
#include <vector>

using namespace std;



class WordKickerInfo : public KitkitGameInfo
{
public:
    virtual std::string getGameName() { return "WordKicker"; }
    virtual std::string getSceneName() { return "WordKickerScene"; }
    
    virtual Scene* createScene(std::string levelID) {
        return WordKicker::WordKickerScene::createScene(levelID);
    }
    
    
    virtual std::vector<std::string> enumerateLevelIDs();
    
    
    
};

class MathKickerInfo : public KitkitGameInfo
{
public:
    virtual std::string getGameName() { return "MathKicker"; }
    virtual std::string getSceneName() { return "MathKickerScene"; }
    
    virtual Scene* createScene(std::string levelID) {
        return WordKicker::WordKickerScene::createScene(levelID, WordKicker::MATHKICKER);
    }
    
    
    virtual std::vector<std::string> enumerateLevelIDs();
    
    
    
};
