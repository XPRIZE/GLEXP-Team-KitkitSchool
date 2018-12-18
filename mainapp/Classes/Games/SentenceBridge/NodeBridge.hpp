//
//  NodeBridge.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#pragma once

#include "SentenceBridgeHeader.hpp"

BEGIN_NS_SENTENCEBRIDGE;

class NodeSentence;
class NodeBlock;
class NodeBridge : public Node
{
public:
	CREATE_FUNC(NodeBridge);
	virtual bool init() override;
	void removeObjects();

	void createBridge(SentenceBridgeProblemStruct problem, float winSizeWidth, bool bRightPillar);

	void blockTouchMoved(string letter, Touch* touch);
	int blockTouchEnded(string letter, Touch* touch, string logWorkPath);
	void showAnswer(NodeBlock* blockNode, int foundSentenceIdx, function<void()> onFinished);
	bool areAllProblemsSolved();
	void debugSolveAll();

private:
	void makeSentence();
	int findTouchedProblemSentenceIdx(string letter, Touch* touch);


public:
	Sprite *_bridgeSprite;
	Size _bridgeSize;

private:
	vector<NodeSentence*> _sentenceNodes;

	SentenceBridgeProblemStruct _problem;
};


END_NS_SENTENCEBRIDGE;