//
//  NodeBridgeMain.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#pragma once

#include "SentenceBridgeHeader.hpp"

BEGIN_NS_SENTENCEBRIDGE;

class NodeBridge;
class NodeBlock;
class NodeCar;
class NodeBridgeMain : public Node
{
public:
	CREATE_FUNC(NodeBridgeMain);
	virtual bool init() override;
	void removeObjects();

	void createBridgeList(const vector<SentenceBridgeProblemStruct> &problems, Size winSize);
	void moveToNextProblem(int nProblemIdx, function<void()> onFinished);
	void moveCarToNext(int nProblemIdx, float duration, bool bIsLast);

	void blockTouchMoved(string letter, Touch* touch);
	int blockTouchEnded(string letter, Touch* touch, string logWorkPath);
	void showAnswer(NodeBlock* blockNode, int foundSentenceIdx, function<void()> onFinished);
	bool areAllProblemsSolved();
	void debugSolveAll();

public:

private:
	vector<NodeBridge*> _bridgeNodes;
	NodeCar* _carNode;
	vector<SentenceBridgeProblemStruct> _problems;
	int _currentProblemIdx;
};

END_NS_SENTENCEBRIDGE;
