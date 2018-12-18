//
//  NodeBridgeMain.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#include "NodeBridgeMain.hpp"
#include "NodeBridge.hpp"
#include "NodeBlock.hpp"
#include "NodeCar.hpp"


BEGIN_NS_SENTENCEBRIDGE;

bool NodeBridgeMain::init()
{
	if (!Node::init()) return false;
	_bridgeNodes.clear();
	_carNode = nullptr;
	return true;
}

void NodeBridgeMain::removeObjects()
{
	_carNode->removeObjects();
	_carNode = nullptr;

	for (auto bridge : _bridgeNodes)
	{
		bridge->removeObjects();
	}
	_bridgeNodes.clear();
	removeAllChildren();
}

void NodeBridgeMain::createBridgeList(const vector<SentenceBridgeProblemStruct> &problems, Size winSize)
{
	_currentProblemIdx = 0;
	_problems = problems;

	_carNode = NodeCar::create();
	addChild(_carNode);

	float bridgePosX = 0;
	for (int i=0; i<_problems.size(); i++)
	{
		auto newBridge = NodeBridge::create();
		newBridge->setPosition(bridgePosX, 0);
		newBridge->createBridge(_problems[i], winSize.width, i == _problems.size() - 1);
		//addChild(newBridge);
		_bridgeNodes.push_back(newBridge);

		bridgePosX += newBridge->_bridgeSize.width;
	}
	

	_carNode->setPosition(0, _bridgeNodes[0]->_bridgeSprite->getPosition().y + _bridgeNodes[0]->_bridgeSize.height / 2);
	_carNode->createCar(-_bridgeNodes[0]->_bridgeSize.width/2);


	Vec2 vPos = Vec2(-_bridgeNodes[0]->_bridgeSize.width / 2,
						_bridgeNodes[0]->_bridgeSprite->getPosition().y + _bridgeNodes[0]->_bridgeSize.height / 2 - 32);
	float fMaxPosX = _bridgeNodes[0]->_bridgeSize.width / 2 + (_bridgeNodes.size() - 1) * _bridgeNodes[0]->_bridgeSize.width;
	while (vPos.x < fMaxPosX)
	{
		auto railSprite = Sprite::create(resourcePath + "bridge-image-rail.png");
		railSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		railSprite->setPosition(vPos);
		addChild(railSprite);
		vPos.x += railSprite->getContentSize().width;
	}
	//setPosition(Vec2(_bridgeNodes[0]->_bridgeSize.width / 2, 0));

	for (auto bridgeNode : _bridgeNodes)
	{
		addChild(bridgeNode);
	}
	/*
	auto move = MoveBy::create(2, -Vec2(_bridgeNodes.back()->getPosition().x, 0));
	auto move_back = move->reverse();
	auto move_ease_in = EaseInOut::create(move->clone(), 2);
	auto move_ease_in_back = EaseInOut::create(move_back->clone(), 2);
	auto delay = DelayTime::create(1);
	auto seq1 = Sequence::create(delay, move_ease_in, delay->clone(), move_ease_in_back, delay->clone(), nullptr);
	runAction(RepeatForever::create(seq1));
	*/
}

void NodeBridgeMain::moveCarToNext(int nProblemIdx, float duration, bool bIsLast)
{
	float fPos = _bridgeNodes[0]->_bridgeSize.width * nProblemIdx - _bridgeNodes[0]->_bridgeSize.width/2;
	_carNode->carMoveTo(duration, fPos, bIsLast);
}

void NodeBridgeMain::moveToNextProblem(int nProblemIdx, function<void()> onFinished)
{
	_currentProblemIdx = nProblemIdx;

	auto move = MoveTo::create(2, -_bridgeNodes[nProblemIdx]->getPosition());
	auto move_ease_in = EaseInOut::create(move->clone(), 2);
	runAction(
		Sequence::create(
			move_ease_in,
			CallFunc::create([onFinished]() {onFinished(); }),
			nullptr
		)
	);
}

void NodeBridgeMain::blockTouchMoved(string letter, Touch* touch)
{
	_bridgeNodes[_currentProblemIdx]->blockTouchMoved(letter, touch);
}

int NodeBridgeMain::blockTouchEnded(string letter, Touch* touch, string logWorkPath)
{
	return _bridgeNodes[_currentProblemIdx]->blockTouchEnded(letter, touch, logWorkPath);
}

void NodeBridgeMain::showAnswer(NodeBlock* blockNode, int foundSentenceIdx, function<void()> onFinished)
{
	_bridgeNodes[_currentProblemIdx]->showAnswer(blockNode, foundSentenceIdx, onFinished);
}

bool NodeBridgeMain::areAllProblemsSolved()
{
	return _bridgeNodes[_currentProblemIdx]->areAllProblemsSolved();
}

void NodeBridgeMain::debugSolveAll()
{
	_bridgeNodes[_currentProblemIdx]->debugSolveAll();
}

END_NS_SENTENCEBRIDGE;
