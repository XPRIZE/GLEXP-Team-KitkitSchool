//
//  NodeBridge.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//


#include "NodeBridge.hpp"
#include "NodeSentence.hpp"
#include "NodeBlock.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_SENTENCEBRIDGE;

bool NodeBridge::init()
{
	if (!Node::init()) return false;
	return true;
}

void NodeBridge::removeObjects()
{
	for (auto sentenceNode : _sentenceNodes)
	{
		sentenceNode->removeObjects();
	}
	_sentenceNodes.clear();

	_bridgeSprite->removeAllChildren();
	removeAllChildren();
}

void NodeBridge::createBridge(SentenceBridgeProblemStruct problem, float winSizeWidth, bool bRightPillar)
{
	_problem = problem;

	_bridgeSprite = Sprite::create(resourcePath + "bridge-image-body.png");
	_bridgeSize = _bridgeSprite->getContentSize();
	_bridgeSize.width = winSizeWidth;
	_bridgeSprite->setContentSize(_bridgeSize);
	_bridgeSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bridgeSprite->setPosition(0, 40);

	auto pillarLeftSprite = Sprite::create(resourcePath + "bridge-image-pillar-left.png");
	pillarLeftSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	pillarLeftSprite->setPosition(-winSizeWidth/2, 40 - _bridgeSize.height / 2);
	addChild(pillarLeftSprite);

	if (bRightPillar)
	{
		auto pillarRightSprite = Sprite::create(resourcePath + "bridge-image-pillar-right.png");
		pillarRightSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		pillarRightSprite->setPosition(winSizeWidth/2, 40 - _bridgeSize.height / 2);
		addChild(pillarRightSprite);
	}

	addChild(_bridgeSprite);
	//reorderChild(bridgeSprite, 0);


	makeSentence();
}

void NodeBridge::makeSentence()
{
	_sentenceNodes.clear();
	_bridgeSprite->removeAllChildren();

	int startPos = -1;
	NodeSentence* newSentenceNode;
	int multilineY = 0;
	for (int i = 0; i < _problem.Sentence.size(); i++)
	{
		auto ProblemSentenceItr = find(_problem.ProblemSentenceIndices.begin(), _problem.ProblemSentenceIndices.end(), i);
		if (ProblemSentenceItr == _problem.ProblemSentenceIndices.end())
		{
			if (startPos < 0)
			{
				startPos = i;
			}
			if (i != _problem.multiLineStartPos)
			{
				continue;
			}
		}

		if (_problem.multiLineStartPos > -1)
		{
			if (i <= _problem.multiLineStartPos)
			{
				multilineY = -1;
			}
			else

			{
				multilineY = 1;
			}
		}
		if (startPos >= 0 && startPos < i)
		{
			newSentenceNode = NodeSentence::create();
			newSentenceNode->createSentence(-1, _problem.Sentence.substr(startPos, i - startPos), multilineY);
			_sentenceNodes.push_back(newSentenceNode);
			newSentenceNode = nullptr;
		}
		startPos = -1;

		if (_problem.multiLineStartPos > -1)
		{
			if (i < _problem.multiLineStartPos)
			{
				multilineY = -1;
			}
			else

			{
				multilineY = 1;
			}
		}
		if (ProblemSentenceItr != _problem.ProblemSentenceIndices.end())
		{
			newSentenceNode = NodeSentence::create();
			newSentenceNode->createSentence(std::distance(_problem.ProblemSentenceIndices.begin(), ProblemSentenceItr), _problem.Sentence.substr(i, 1), multilineY);
			_sentenceNodes.push_back(newSentenceNode);
			newSentenceNode = nullptr;
		}
		else if (i == _problem.multiLineStartPos)
		{
			startPos = i;
		}
	}

	if (startPos >= 0)
	{
		if (_problem.multiLineStartPos > -1)
		{
			multilineY = 1;
		}

		newSentenceNode = NodeSentence::create();
		newSentenceNode->createSentence(-1, _problem.Sentence.substr(startPos, _problem.Sentence.size() - startPos), multilineY);
		_sentenceNodes.push_back(newSentenceNode);
		newSentenceNode = nullptr;
	}
	startPos = -1;


	float fTotalWidth_1 = 0;
	float fTotalWidth_2 = 0;
	for (auto sentenceNode : _sentenceNodes)
	{
		if (sentenceNode->_multilineY <= 0)
		{
			fTotalWidth_1 += sentenceNode->getSentenceWidth();
		}
		else
		{
			fTotalWidth_2 += sentenceNode->getSentenceWidth();
		}
	}
	float fTotalWidth = max(fTotalWidth_1, fTotalWidth_2);

	float fPosX_1 = _bridgeSize.width / 2 - fTotalWidth / 2;
	float fPosX_2 = _bridgeSize.width / 2 - fTotalWidth / 2;
	float fPosY = _bridgeSize.height / 2;
	//float fPosY_label = _bridgeSize.height / 2 - 30;
	//float fPosY_sprite = _bridgeSize.height / 2 + 40;
	for (auto sentenceNode : _sentenceNodes)
	{
		float pieceWidth = sentenceNode->getSentenceWidth();
		
		if (sentenceNode->_multilineY <= 0)
		{
			sentenceNode->setPosition(fPosX_1 + pieceWidth / 2, fPosY);
			fPosX_1 += pieceWidth;
		}
		else
		{
			sentenceNode->setPosition(fPosX_2 + pieceWidth / 2, fPosY);
			fPosX_2 += pieceWidth;
		}

		_bridgeSprite->addChild(sentenceNode);
	}
}

int NodeBridge::findTouchedProblemSentenceIdx(string letter, Touch* touch)
{
	for (int i = 0; i<_sentenceNodes.size(); i++)
	{
		if (_sentenceNodes[i]->isTouchedProblemSentence(letter, touch))
		{
			return i;
		}
	}

	return -1;
}

void NodeBridge::blockTouchMoved(string letter, Touch* touch)
{
	auto foundSentenceIdx = findTouchedProblemSentenceIdx(letter, touch);
	
	for (int i = 0; i < _sentenceNodes.size(); i++)
	{
		if (i == foundSentenceIdx)
		{
			_sentenceNodes[i]->showSelectedEffect();
		}
		else
		{
			_sentenceNodes[i]->hideSelectedEffect();
		}
	}
}

int NodeBridge::blockTouchEnded(string letter, Touch* touch, string logWorkPath)
{
	auto foundSentenceIdx = findTouchedProblemSentenceIdx(letter, touch);

	if (foundSentenceIdx >= 0)
	{
		StrictLogManager::shared()->game_Peek_Answer("SentenceBridge", logWorkPath, letter, _sentenceNodes[foundSentenceIdx]->_strSentence);
	}
	else
	{
		StrictLogManager::shared()->game_Peek_Answer("SentenceBridge", logWorkPath, letter, "None");
	}

	for (auto sentenceNode : _sentenceNodes)
	{
		sentenceNode->hideSelectedEffect();
	}

	if (foundSentenceIdx >= 0 && _sentenceNodes[foundSentenceIdx]->_strSentence == letter)
	{
		/*
		auto eraseIdx = _sentenceNodes[foundSentenceIdx]->_problemSentenceIdx;
		_problem.ProblemSentenceIndices.erase(_problem.ProblemSentenceIndices.begin() + eraseIdx);
		makeSentence();

		return true;
		*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right.mp3");
#else
		GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/right.m4a");
#endif

		return foundSentenceIdx;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/matrix_wrongmove.mp3");
#else
	GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/matrix_wrongmove.m4a");
#endif

	return -1;
}

void NodeBridge::showAnswer(NodeBlock* blockNode, int foundSentenceIdx, function<void()> onFinished)
{
	blockNode->blockMoveToWorldPosition(0.1f, _sentenceNodes[foundSentenceIdx]->getSlotWorldPos());
	_sentenceNodes[foundSentenceIdx]->hideSentenceSpriteBG(0.5f);
	runAction(
		Sequence::create(
			DelayTime::create(0.5f),
			CallFunc::create([this, blockNode, foundSentenceIdx, onFinished]() {
				blockNode->blockResetPosition();
				auto eraseIdx = _sentenceNodes[foundSentenceIdx]->_problemSentenceIdx;
				_problem.ProblemSentenceIndices.erase(_problem.ProblemSentenceIndices.begin() + eraseIdx);
				makeSentence();
				onFinished();
			}),
			nullptr
		)
	);
}

bool NodeBridge::areAllProblemsSolved()
{
	return _problem.ProblemSentenceIndices.size() < 1;
}

void NodeBridge::debugSolveAll()
{
	_problem.ProblemSentenceIndices.clear();
	makeSentence();
}

END_NS_SENTENCEBRIDGE;
