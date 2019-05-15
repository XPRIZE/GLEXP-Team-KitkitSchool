//
//  AnimalPuzzleInfo.hpp
//  KitkitSchool
//
//  Created by Gunho Lee on 7/14/18.
//

#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "Common/Basic/KitkitGameInfo.hpp"

#include "AnimalPuzzleScene.hpp"

#include <string>
#include <vector>

using namespace std;



class AnimalPuzzleInfo : public KitkitGameInfo
{
public:
    virtual std::string getGameName() { return "AnimalPuzzle"; }
    virtual std::string getSceneName() { return "AnimalPuzzleScene"; }
    
    virtual Scene* createScene(std::string levelID) {
        return AnimalPuzzleScene::createScene(levelID);
    }
    
    
    virtual std::vector<std::string> enumerateLevelIDs();
    
    
    
};

