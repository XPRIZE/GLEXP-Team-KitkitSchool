/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
Created		:	2018.02.08

Author		:	Mikey.p

Purpose		:	WhatIsThis

****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cocos2d.h"
#include <vector>

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;

struct WhatIsThisCardData
{
	std::string answer;
	std::string pic;
	std::string sound;
	float soundDuration = 0.f;
};

struct WhatIsThisData
{
	std::string language;
	int level = 0;
	std::string mode;
	std::string problem;
	WhatIsThisCardData answer;
	std::vector<WhatIsThisCardData> wrong_answer;
};

class WhatIsThisLayer : public Layer
{
public:
	CREATE_FUNC(WhatIsThisLayer);
	virtual bool init() override;
    void initUI();
    
    void SetLevel(const int& level) { stageLevel = level; }
    int GetLevel() const { return stageLevel; }
    std::vector<int> getLevels();
    
	int GetTotalProblemCount() const { return totalProblemCount; }
	int GetCurrentProblem() const { return totalProblemCount - currentDatas.size(); }
	
	void StartGame();
	void NextGame();
	void EndGame();

	void LoadDatas();

private:
    int stageLevel = -1;
	int totalProblemCount = 0;
    int currentProblemIndex = 0;
    int _maxLevel;

    Node* topCardNode = nullptr;
	Node* failCardNode = nullptr;
    std::vector<Node*> downCardNodes;
	std::set<std::string> holdTouch;
	std::map<int, std::vector<WhatIsThisData>> whatIsThisDatas;
	std::vector<WhatIsThisData> currentDatas;
    
    ProgressIndicator *_progressBar;
    std::string makeWorkPath();

};

class WhatIsThisScene : public Scene
{
public:
	CREATE_FUNC(WhatIsThisScene);

	virtual bool init() override;

    void SetLevelAndStart(const int& level)
    {
        if (mainLayer)
        {
            mainLayer->SetLevel(level);
            mainLayer->StartGame();
        }
    }
    int GetLevel()
    {
        if (mainLayer)
            return mainLayer->GetLevel();
        
        return -1;
    }
    Layer* GetMainLayer() { return mainLayer; }

private:
    WhatIsThisLayer* mainLayer = nullptr;
    
};
