/////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
 Created        :    2018.02.21
 
 Author        :    Mikey.p
 
 Purpose        :    FeedingTime Scene
 
 ****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#include "FeedingTimeScene.h"

#include "Utils/MMUtil.h"
#include "FeedingTimeNode.h"

#include "CCAppController.hpp"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"


USING_NS_CC;

#define CARD_COUNT 3
#define NEXT_DELAY 0.75f
#define CARD_DISTANCE_TIME 0.125f

#define SUCESS_WAIT_TIME_1 1.3f
#define SUCESS_WAIT_TIME_2 2.25f
#define FAIL_WAIT_TIME 1.4f

/////////////////////////////////////////////////////////////////////////////////////////

bool FeedingTimeLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}

void FeedingTimeLayer::initUI() {
    
    setName("FeedingTimeLayer");
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = { visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y };

    // 배경설정1
    if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_bg.png"))
    {
		auto size = sprite->getContentSize();
		sprite->setPosition({ center.x, visibleSize.height - size.height / 2.f });
		if (size.width < visibleSize.width)
			sprite->setContentSize({ visibleSize.width, size.height});
        addChild(sprite, 0, "BG1");
    }

	auto centerNode = FTCenterNode::create();
	centerNode->setPosition({ center.x, center.y });
	addChild(centerNode, 0, "centerNode");
	childNode.push_back(centerNode);

  auto leftNode = FTCellNode::create();
	leftNode->setPosition({ center.x - 927.f, center.y + 220 });
	addChild(leftNode, 0, "leftNode");
	childNode.push_back(leftNode);

	auto rightNode = FTCellNode::create();
	rightNode->setPosition({ center.x + 927.f, center.y + 220 });
	addChild(rightNode, 0, "rightNode");
	childNode.push_back(rightNode);

	// 나무
	if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_wood_left.png"))
	{
		sprite->setPosition({ center.x - 899, center.y -7});
		addChild(sprite, 0, "BG1");
	}
	if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_wood_right.png"))
	{
		sprite->setPosition({ center.x + 886, center.y -7 });
		addChild(sprite, 0, "BG1");
	}

	// 배경설정2
	if (auto sprite = Sprite::create("FeedingTime/whichisbigger_rsc_grass.png"))
	{
		auto size = sprite->getContentSize();
		sprite->setPosition({ center.x, size.height / 2.f });
		if (size.width < visibleSize.width)
			sprite->setContentSize({ visibleSize.width, size.height });
		addChild(sprite, 0, "BG2");
	}

	auto lagerNode = FTRelationNode::create();
	lagerNode->setPosition({ center.x - 773, center.y - 526 });
	addChild(lagerNode, 0, "lagerNode");
	childNode.push_back(lagerNode);

	auto smallerNode = FTRelationNode::create();
	smallerNode->setPosition({ center.x + 773, center.y - 526 });
	addChild(smallerNode, 0, "smallerNode");
	childNode.push_back(smallerNode);

	auto equalNode = FTRelationNode::create();
	equalNode->setPosition({ center.x, center.y - 526 });
	addChild(equalNode, 0, "equalNode");
	childNode.push_back(equalNode);
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, visibleSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(visibleSize.width/2, visibleSize.height - _progressBar->getContentSize().height-10));
    addChild(_progressBar);
    
    if (UserManager::getInstance()->isDebugMode()) {
        auto skip = ui::Button::create();
        skip->setTitleFontSize(100);
        skip->setTitleText("Skip");
        skip->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        skip->setPosition(Vec2(visibleSize.width-25, visibleSize.height-25));
        addChild(skip);
        skip->addTouchEventListener([this](Ref*,ui::Widget::TouchEventType e) {
            if (e == ui::Widget::TouchEventType::ENDED) {
                NextGame();
            }
        });
        
    }
    
    LoadDatas();
}

void FeedingTimeLayer::StartGame()
{
    holdTouch.insert("StartGame");
    
    if (currentDatas.empty())
    {
        currentDatas = FeedingTimeDatas[stageLevel];
        totalProblemCount = currentDatas[0].problemCount;
        _progressBar->setMax(totalProblemCount);

		std::random_device rd;
		std::mt19937 g(rd());
		std::vector<int> randomIndex;

		std::shuffle(currentDatas.begin(), currentDatas.end(), g);

		while (currentDatas.size() > totalProblemCount)
		{
			currentDatas.pop_back();
		}
    }
    
    if (currentDatas.empty())
        return;
    
    auto currentData = currentDatas.front();
    currentDatas.erase(currentDatas.begin());
    
    _progressBar->setCurrent(++currentProblemIndex, false);
    
	auto GetNumber = [](const FeedingTimeCellData& data)
	{
		if (data.operatorType == "+")
			return data.count + data.count2;
		if (data.operatorType == "*")
			return data.count * data.count2;
		if (data.operatorType == "/")
			return data.count / data.count2;
		if (data.operatorType == "-")
			return data.count - data.count2;

		return data.count;
	};

	int leftNumber = GetNumber(currentData.leftData);
	int rightNumber = GetNumber(currentData.rightData);
	int resultIndex = leftNumber > rightNumber ? 0 : leftNumber < rightNumber ? 2 : 1;
	FTRelationNode::Type resultType = FTRelationNode::Type(resultIndex);

	if (_armatureFile.empty())
		_armatureFile = GenArmatureFile();

	if (auto node = dynamic_cast<FTCenterNode*>(getChildByName("centerNode")))
		node->SetData(resultIndex, _armatureFile);

	if (auto node = dynamic_cast<FTCellNode*>(getChildByName("leftNode")))
		node->SetData(FTCellNode::Side::Left, currentData.leftData);

	if (auto node = dynamic_cast<FTCellNode*>(getChildByName("rightNode")))
		node->SetData(FTCellNode::Side::Right, currentData.rightData);

	if (auto node = dynamic_cast<FTRelationNode*>(getChildByName("lagerNode")))
	{
		node->SetClicked(false);
		node->SetData(FTRelationNode::Type::Lager, resultType, leftNumber);
		node->SetCallBack([this, leftNumber, rightNumber, node]
		{
			if (holdTouch.size() > 0)
				return;

            string correctAnswer = (leftNumber > rightNumber ? ">" : (leftNumber == rightNumber ? "==" : (leftNumber < rightNumber ? "<" : "None")));
            StrictLogManager::shared()->game_Peek_Answer("FeedingTime", MakeWorkPath(), ">", correctAnswer);

			node->SetClicked(true);
            if (leftNumber > rightNumber) {
				Success(0);
            }
            else {
				Fail(0);
            }
		});
	}

	if (auto node = dynamic_cast<FTRelationNode*>(getChildByName("smallerNode")))
	{
		node->SetClicked(false);
		node->SetData(FTRelationNode::Type::Smaller, resultType, rightNumber);
		node->SetCallBack([this, leftNumber, rightNumber, node]
		{
			if (holdTouch.size() > 0)
				return;

            string correctAnswer = (leftNumber > rightNumber ? ">" : (leftNumber == rightNumber ? "==" : (leftNumber < rightNumber ? "<" : "None")));
            StrictLogManager::shared()->game_Peek_Answer("FeedingTime", MakeWorkPath(), "<", correctAnswer);

			node->SetClicked(true);
			if (leftNumber < rightNumber)
				Success(2);
			else
				Fail(2);
		});
	}

	if (auto node = dynamic_cast<FTRelationNode*>(getChildByName("equalNode")))
	{
		node->SetClicked(false);
		node->SetData(FTRelationNode::Type::Equal, resultType, 0);
		node->SetCallBack([this, leftNumber, rightNumber, node]
		{
			if (holdTouch.size() > 0)
				return;

            string correctAnswer = (leftNumber > rightNumber ? ">" : (leftNumber == rightNumber ? "==" : (leftNumber < rightNumber ? "<" : "None")));
            StrictLogManager::shared()->game_Peek_Answer("FeedingTime", MakeWorkPath(), "==", correctAnswer);

			node->SetClicked(true);
			if (leftNumber == rightNumber)
				Success(1);
			else
				Fail(1);
		});
	}

	for (auto node : childNode)
		node->StartGame();

    Vector<FiniteTimeAction*> actions;

	actions.pushBack(DelayTime::create(NEXT_DELAY));
    actions.pushBack(CallFunc::create([this]
    {
        holdTouch.erase("StartGame");
    }));
    
    runAction(Sequence::create(actions));
}

void FeedingTimeLayer::NextGame()
{
    holdTouch.insert("NextGame");
    
    if (currentDatas.empty())
    {
        holdTouch.erase("NextGame");
        EndGame();
        return;
    }

	_armatureFile = GenArmatureFile();

	if (auto node = dynamic_cast<FTCenterNode*>(getChildByName("centerNode")))
		node->SetNextArmatureFile(_armatureFile);

	for (auto node : childNode)
		node->NextGame();

    Vector<FiniteTimeAction*> actions;
  
    actions.pushBack(DelayTime::create(NEXT_DELAY));
    actions.pushBack(CallFunc::create([this]
                                      {
                                          holdTouch.erase("NextGame");
                                          StartGame();
                                      }));
    
    runAction(Sequence::create(actions));
}

void FeedingTimeLayer::EndGame()
{
    holdTouch.insert("EndGame");
    // TODO 게임 끝처리
    
    _progressBar->setCurrent(totalProblemCount+1, false);

    this->scheduleOnce([this](float){
        auto CP = CompletePopup::create();
        CP->show(0.3, [this] {
            CCAppController::sharedAppController()->handleGameComplete(1);
        });
    }, 0.3, "onGameComplete");
    
	for (auto node : childNode)
		node->EndGame();

    holdTouch.erase("EndGame");
}

void FeedingTimeLayer::Success(int index)
{
	holdTouch.insert("Success");

	for (auto node : childNode)
		node->Success();

	Vector<FiniteTimeAction*> actions;
	actions.pushBack(DelayTime::create(SUCESS_WAIT_TIME_1));
	actions.pushBack(CallFunc::create([this, index]
	{
		if (auto node = dynamic_cast<FTCellNode*>(getChildByName("leftNode")))
		{
			if (index == 0)
				node->EatIt();
			else if (index == 1)
				node->EatAll();
			else
				node->FallDown();
		}

		if (auto node = dynamic_cast<FTCellNode*>(getChildByName("rightNode")))
		{
			if (index == 2)
				node->EatIt();
			else if (index == 1)
				node->EatAll();
			else
				node->FallDown();
		}
	}));
	actions.pushBack(DelayTime::create(SUCESS_WAIT_TIME_2));
	actions.pushBack(CallFunc::create([this]
	{
		holdTouch.erase("Success");
		NextGame();
	}));

	runAction(Sequence::create(actions));
}

void FeedingTimeLayer::Fail(int index)
{
	holdTouch.insert("Fail");

	for (auto node : childNode)
		node->Fail();

	Vector<FiniteTimeAction*> actions;

	actions.pushBack(DelayTime::create(FAIL_WAIT_TIME));
	actions.pushBack(CallFunc::create([this]
	{
		holdTouch.erase("Fail");
	}));

	runAction(Sequence::create(actions));
}

void FeedingTimeLayer::LoadDatas()
{
    std::vector <std::vector<std::string>> result;
    std::string s = FileUtils::getInstance()->getStringFromFile("FeedingTime/FeedingTime.tsv");
    MMUtil::ParseTSV(result, s);
    
    FeedingTimeDatas.clear();
    std::map<std::string, int> paser;
    
    auto SetParser = [&paser] (std::vector<std::string>& firstRow)
    {
        paser.clear();
        for (int i = 0; i < firstRow.size(); i++)
        {
            paser[firstRow[i]] = i;
        }
    };
    
    bool firstLine = true;
    for (int i = 0; i < result.size(); i++)
    {
        auto& line = result[i];
        if (line.empty())
            continue;
        
		if (line[0].substr(0, 1) == "#")
		{
			if (firstLine && i + 1 < result.size() && result[i+1].size() > 0 && result[i + 1][0].substr(0 ,1) != "#")
			{
				line[0] = line[0].substr(1);
			}
			else
				continue;
		}
        
        if (firstLine)
        {
            SetParser(line);
            firstLine = false;
            continue;
        }
        
        if (line.size() < paser.size())
            continue;
        
        FeedingTimeData data;
        data.level = atoi(line[paser["level"]].c_str());
		data.problemCount = atoi(line[paser["problemCount"]].c_str());
        data.problem = line[paser["problem"]];
        
        FeedingTimeCellData cardData;
        cardData.count = atoi(line[paser["left1 count"]].c_str());
		cardData.count2 = atoi(line[paser["left2 count"]].c_str());
        cardData.type = line[paser["left property"]];
		cardData.operatorType = line[paser["left operator"]];
        data.leftData = cardData;
        cardData.count = atoi(line[paser["right1 count"]].c_str());
		cardData.count2 = atoi(line[paser["right2 count"]].c_str());
		cardData.type = line[paser["right property"]];
		cardData.operatorType = line[paser["right operator"]];
		data.rightData = cardData;
        
        if (data.level > _maxLevel) _maxLevel = data.level;
        FeedingTimeDatas[data.level].push_back(data);
    }
}

std::vector<int> FeedingTimeLayer::getLevels() {
    LoadDatas();
    
    vector<int> ret;
    ret.clear();
    
    for (int i=1; i<=_maxLevel; i++) {
        ret.push_back(i);
    }
    return ret;
}


std::string FeedingTimeLayer::GenArmatureFile()
{
	std::vector<std::string> armatureFiles =
	{
		"FeedingTime/Croc/Croc.ExportJson",
		"FeedingTime/Frog/Frog.ExportJson",
		"FeedingTime/Hippo/Hippo.ExportJson"
	};

	return armatureFiles[random(0, (int)armatureFiles.size() - 1)];
}

std::string FeedingTimeLayer::MakeWorkPath() {
    stringstream ss;
    ss << "FeedingTime";
    ss << "/" << "level-" << stageLevel;
    ss << "/" << "work-" << currentProblemIndex;
    return ss.str();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool FeedingTimeScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    setName("FeedingTimeScene");
    
    mainLayer = FeedingTimeLayer::create();
    mainLayer->initUI();
    
    addChild(mainLayer);
    
    return true;
}
