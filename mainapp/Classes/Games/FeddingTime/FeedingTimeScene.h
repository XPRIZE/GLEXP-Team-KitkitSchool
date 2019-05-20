/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
 Created        :    2018.02.21
 
 Author        :    Mikey.p
 
 Purpose        :    FeedingTime Scene
 
 ****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cocos2d.h"

#include "Common/Controls/ProgressIndicator.hpp"

USING_NS_CC;

class FeedingTimeNode;

struct FeedingTimeCellData
{
	std::string type;
    int count = 0;
	int count2 = 0;
	std::string operatorType;
};

struct FeedingTimeData
{
    int level = 0;
	int problemCount = 0;
    std::string problem;
    FeedingTimeCellData leftData;
    FeedingTimeCellData rightData;
};

class FeedingTimeLayer : public Layer
{
public:
    CREATE_FUNC(FeedingTimeLayer);
    virtual bool init() override;
    void initUI();

    void SetLevel(const int& level) { stageLevel = level; }
    int GetLevel() const { return stageLevel; }
    
    int GetTotalProblemCount() const { return totalProblemCount; }
    int GetCurrentProblem() const { return totalProblemCount - currentDatas.size(); }
    std::vector<int> getLevels();

    void StartGame();
    void NextGame();
    void EndGame();

private:
	void Success(int index);
	void Fail(int index);

	void LoadDatas();

	std::string GenArmatureFile();
    std::string MakeWorkPath();

    int stageLevel = -1;
    int totalProblemCount = 0;
    int currentProblemIndex = 0;
    int _maxLevel;
    
    Node* topCardNode = nullptr;
    std::vector<Node*> downCardNodes;
    std::set<std::string> holdTouch;
    std::map<int, std::vector<FeedingTimeData>> FeedingTimeDatas;
    std::vector<FeedingTimeData> currentDatas;
	std::vector<FeedingTimeNode*> childNode;
	std::string _armatureFile;
    
    ProgressIndicator *_progressBar;
};

class FeedingTimeScene : public Scene
{
public:
    CREATE_FUNC(FeedingTimeScene);
    
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
    FeedingTimeLayer* mainLayer = nullptr;
    
};

