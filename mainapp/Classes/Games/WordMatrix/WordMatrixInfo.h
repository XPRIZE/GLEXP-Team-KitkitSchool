#pragma once

#include "WordMatrixScene.h"
#include "Common/Basic/KitkitGameInfo.hpp"

USING_NS_CC;
using namespace std;

class WordMatrixInfo : public KitkitGameInfo
{
public:
    virtual std::string getGameName() { return "WordMatrix"; }
    virtual std::string getSceneName() { return "WordMatrixScene"; }
    
    virtual Scene* createScene(std::string levelID)
    {
        return WordMatrixScene::createScene(levelID);
    }
    
    virtual std::vector<std::string> enumerateLevelIDs();

private:
    int getMaxLevel();
};
