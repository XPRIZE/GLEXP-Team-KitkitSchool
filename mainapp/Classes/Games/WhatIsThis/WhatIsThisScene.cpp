 /////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************
Created		:	2018.02.08

Author		:	Mikey.p

Purpose		:	WhatIsThisScene

****************************************************************************************/
/////////////////////////////////////////////////////////////////////////////////////////

#include <Managers/VoiceMoldManager.h>
#include "WhatIsThisScene.h"

#include "CCAppController.hpp"

#include "Utils/MMUtil.h"
#include "Utils/TodoUtil.h"
#include "WhatIsThisCard.h"
#include "Managers/GameSoundManager.h"
#include "Managers/UserManager.hpp"
#include "Managers/StrictLogManager.h"

#include "Common/Controls/TodoSchoolBackButton.hpp"
#include "Common/Controls/CompletePopup.hpp"

USING_NS_CC;

#define CARD_COUNT 3
#define NEXT_DELAY 0.75f
#define CARD_DISTANCE_TIME 0.125f
/////////////////////////////////////////////////////////////////////////////////////////

bool WhatIsThisLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
    return true;
}

void WhatIsThisLayer::initUI() {
    
	setName("WhatIsThisLayer");

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 배경설정
    if (auto sprite = Sprite::create("WhatIsThis/Images/showandtel_image_background.png"))
    {
        sprite->setContentSize(getContentSize());
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        addChild(sprite, 0);
    }
    
    auto backButton = TodoSchoolBackButton::create();
    backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backButton->setPosition(Vec2(25, visibleSize.height-25));
    addChild(backButton);
    
    _progressBar = ProgressIndicator::create();
    _progressBar->setPosition(Vec2(visibleSize.width/2, visibleSize.height - _progressBar->getContentSize().height-20));
    addChild(_progressBar);
    
    // 상단 카드 초기화
    auto topCard = STTopCard::create();
	topCard->SetBasePos(Vec2(visibleSize.width / 2, visibleSize.height - 566));
	topCard->setPosition(visibleSize * 2.f);
	topCardNode = topCard;
    addChild(topCardNode);
	auto failCard = STTopCard::create();
	failCard->SetFail(true);
	failCard->SetBasePos(Vec2(visibleSize.width / 2, visibleSize.height - 566));
	failCard->setPosition(visibleSize * 2.f);
	failCardNode = failCard;
	addChild(failCardNode);

	//하단 카드 초기화
    downCardNodes.clear();
    for (int i = 0; i < CARD_COUNT; i++)
    {
        auto downCard = STDownCard::create();
		downCard->SetBasePos(Vec2(visibleSize.width / 2 + 838 * (i - 1), 345));
		downCard->setPosition(visibleSize * 2.f);
        downCardNodes.push_back(downCard);
        addChild(downCard);
    }
    
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

void WhatIsThisLayer::StartGame()
{
	holdTouch.insert("StartGame");

	// 이번 데이터 설정
	if (currentDatas.empty())
	{
		currentDatas = whatIsThisDatas[stageLevel];
        random_shuffle(currentDatas.begin(), currentDatas.end());
		totalProblemCount = currentDatas.size();
        _progressBar->setMax(totalProblemCount);
	}

	auto currentData = currentDatas.front();
	currentDatas.erase(currentDatas.begin());
    
    _progressBar->setCurrent(++currentProblemIndex, false);
    
	GameSoundManager::getInstance()->playEffectSound("WhatIsThis/Sounds/ShowAndTell_SFX_Card_shuffle_short.m4a");
	//VoiceMoldManager::shared()->speak("india");

	// 상단 카드 설정
	if (auto card = dynamic_cast<STTopCard*>(topCardNode))
	{
		card->SetCard(currentData.answer, currentData.mode == "ChooseSentense" ? WhatIsThisCard::CardType::Image : WhatIsThisCard::CardType::Text);
		card->ThrowIn();
	}

	int currentCount = currentData.wrong_answer.size() + 1;

	Vector<FiniteTimeAction*> actions;

	std::random_device rd;
	std::mt19937 g(rd());
	std::vector<int> randomIndex;

	// 하단 카드 진입
	for (int i = 0; i < currentCount; i++)
	{
		randomIndex.push_back(i);

		if (auto card = dynamic_cast<WhatIsThisCard*>(downCardNodes[currentCount - i - 1]))
		{
			actions.pushBack(DelayTime::create(CARD_DISTANCE_TIME));
			actions.pushBack(CallFunc::create([card]
			{
				card->ThrowIn();
			}));
		}
	}

	// 카드 섞기
	std::shuffle(randomIndex.begin(), randomIndex.end(), g);

	// 하단 카드 설정
	auto visibleSize = Director::getInstance()->getVisibleSize();
	for (int i = 0; i < currentCount; i++)
	{
		auto currentCardID = randomIndex[i];
        auto correctCardID = randomIndex[currentCount-1];
		if (auto card = dynamic_cast<STDownCard*>(downCardNodes[currentCardID]))
		{
			if (currentData.mode == "ChooseSentense")
				card->SetBasePos(Vec2(visibleSize.width / 2, 345 + 219 * (currentCardID - (currentCount - 1) / 2.f)));
			else
			{
				if (currentCount > 2)
					card->SetBasePos(Vec2(visibleSize.width / 2 + 838 * (currentCardID - (currentCount - 1) / 2.f), 345));
				else
					card->SetBasePos(Vec2(visibleSize.width / 2 - 556 + (currentCardID * 1112), 345));

			}

			if (i == currentCount - 1)
			{
				// 정답 카드 설정
				card->SetCard(currentData.answer, currentData.mode == "ChooseSentense" ? WhatIsThisCard::CardType::Text : WhatIsThisCard::CardType::Image);
				// 카드 터치 처리
				card->SetClickEvent([this, currentCardID, correctCardID, currentCount]
				{
					if (holdTouch.size() > 0)
						return;

					holdTouch.insert("CardFlip");
                    StrictLogManager::shared()->game_Peek_Answer("WhatIsThis", makeWorkPath(), TodoUtil::itos(currentCardID), TodoUtil::itos(correctCardID));

					for (int i = 0; i < currentCount; i++)
					{
						if (i == currentCardID)
						{
							dynamic_cast<STDownCard*>(downCardNodes[i])->Flip(true, [this]
							{
								NextGame();
								holdTouch.erase("CardFlip");
							});
						}
						else
						{
							dynamic_cast<STDownCard*>(downCardNodes[i])->Flip(false, nullptr);
						}
					}
				});
			}
			else
			{
				// 오답 카드 설정
				card->SetCard(currentData.wrong_answer[i], currentData.mode == "ChooseSentense" ? WhatIsThisCard::CardType::Text : WhatIsThisCard::CardType::Image);
				// 카드 터치 처리
				card->SetClickEvent([this, card, currentCount, currentData, i, currentCardID, correctCardID, visibleSize]
				{
					if (holdTouch.size() > 0)
						return;

					holdTouch.insert("CardFail");
                    StrictLogManager::shared()->game_Peek_Answer("WhatIsThis", makeWorkPath(), TodoUtil::itos(currentCardID), TodoUtil::itos(correctCardID));

					if (auto failCard = dynamic_cast<STTopCard*>(failCardNode))
					{
						failCard->setPosition(failCard->GetBasePos() + Vec2(0.f, visibleSize.height));
						failCard->SetCard(currentData.wrong_answer[i], currentData.mode == "ChooseSentense" ? WhatIsThisCard::CardType::Image : WhatIsThisCard::CardType::Text);

						Vector<FiniteTimeAction*> actions;
						actions.pushBack(EaseOut::create(MoveTo::create(0.35f, failCard->GetBasePos()), 2.f));
						actions.pushBack(DelayTime::create(MAX(1.3f, card->_soundDuration-0.7)));
						actions.pushBack(EaseIn::create(MoveTo::create(0.35f, failCard->GetBasePos() + Vec2(0.f, visibleSize.height)), 2.f));

						failCard->runAction(Sequence::create(actions));
					}

					for (int i = 0; i < currentCount; i++)
					{
						if (i == currentCardID)
						{
							dynamic_cast<STDownCard*>(downCardNodes[i])->Fail(true, [this]
							{
								holdTouch.erase("CardFail");
							});
						}
						else
						{
							dynamic_cast<STDownCard*>(downCardNodes[i])->Fail(false, nullptr);
						}
					}
				});
			}
		}
	}

	actions.pushBack(CallFunc::create([this]
	{
		holdTouch.erase("StartGame");
	}));

	runAction(Sequence::create(actions));
}

void WhatIsThisLayer::NextGame()
{
	holdTouch.insert("NextGame");

	if (currentDatas.empty())
	{
		holdTouch.erase("NextGame");
		EndGame();
		return;
	}

	if (auto card = dynamic_cast<WhatIsThisCard*>(topCardNode))
	{
		card->ThrowOut();
	}

	Vector<FiniteTimeAction*> actions;
	for (int i = 0; i < CARD_COUNT; i++)
	{
		if (auto card = dynamic_cast<WhatIsThisCard*>(downCardNodes[CARD_COUNT - i - 1]))
		{
			actions.pushBack(DelayTime::create(CARD_DISTANCE_TIME));
			actions.pushBack(CallFunc::create([card]
			{
				card->ThrowOut();
			}));
		}
	}

	actions.pushBack(DelayTime::create(NEXT_DELAY));
	actions.pushBack(CallFunc::create([this]
	{
		holdTouch.erase("NextGame");
        StartGame();
	}));

	runAction(Sequence::create(actions));
}

void WhatIsThisLayer::EndGame()
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

	holdTouch.erase("EndGame");
}

void WhatIsThisLayer::LoadDatas()
{
    std::vector <std::vector<std::string>> result;
    std::string s = FileUtils::getInstance()->getStringFromFile("WhatIsThis/WhatIsThis.tsv");
	MMUtil::ParseTSV(result, s);

	whatIsThisDatas.clear();
    _maxLevel = 0;
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
			if (firstLine && i + 1 < result.size() && result[i + 1].size() > 0 && result[i + 1][0].substr(0, 1) != "#")
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
        
		WhatIsThisData data;
		data.language = line[paser["language"]];
		data.level = atoi(line[paser["level"]].c_str());
		data.mode = line[paser["mode"]];
		data.problem = line[paser["problem"]];

		WhatIsThisCardData cardData;
		cardData.answer = MMUtil::ReplaceReturn(line[paser["answer"]]);
		cardData.pic = line[paser["answer pic"]];
		cardData.sound = line[paser["answer sound"]];

		cardData.soundDuration = (float)atof(line[paser["answer sound duration"]].c_str());
		data.answer = cardData;
 //  float speeching(string text)
  // {
   	//	 VoiceMoldManager::shared()->speak(text);

  // }
		for (int i = 1; i < CARD_COUNT; i++)
		{
			WhatIsThisCardData cardData;
			cardData.answer = MMUtil::ReplaceReturn(line[paser[StringUtils::format("wrong answer %d", i)]]);
			cardData.pic = line[paser[StringUtils::format("wrong answer %d pic", i)]];
			cardData.sound = line[paser[StringUtils::format("wrong answer %d sound", i)]];


			cardData.soundDuration = (float)atof(line[paser[StringUtils::format("wrong answer %d sound duration", i)]].c_str());
			if (!cardData.answer.empty() || !cardData.pic.empty())
				data.wrong_answer.push_back(cardData);
		}
        if (data.level > _maxLevel) _maxLevel = data.level;

		whatIsThisDatas[data.level].push_back(data);

	}
}

std::vector<int> WhatIsThisLayer::getLevels() {
    LoadDatas();
    
    vector<int> ret;
    ret.clear();
    
    for (int i=1; i<=_maxLevel; i++) {
        ret.push_back(i);
    }
    return ret;
}

std::string WhatIsThisLayer::makeWorkPath() {
    stringstream ss;
    ss << "/" << "WhatIsThis";
    ss << "/" << "level-" << stageLevel << "-0";
    ss << "/" << "work-" << currentProblemIndex;
    return ss.str();
}

/////////////////////////////////////////////////////////////////////////////////////////

bool WhatIsThisScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	setName(" WhatIsThisScene");

	mainLayer = WhatIsThisLayer::create();
    mainLayer->initUI();

	addChild(mainLayer);

    return true;
}
