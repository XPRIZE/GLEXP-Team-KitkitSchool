//
//  NodeSentence.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#pragma once

#include "SentenceBridgeHeader.hpp"

BEGIN_NS_SENTENCEBRIDGE;

class NodeSentence : public Node
{
public:
	CREATE_FUNC(NodeSentence);
	virtual bool init() override;
	void removeObjects();

	void createSentence(int problemSentenceIdx, string strSentence, int multilineY);
	float getSentenceWidth();

	bool isTouchedProblemSentence(string letter, Touch* touch);

	void showSelectedEffect();
	void hideSelectedEffect();
	Vec2 getSlotWorldPos();
	void hideSentenceSpriteBG(float duration);


private:
	void makeSentence();


public:
	string _strSentence;
	int _problemSentenceIdx;
	bool _isSymbolProblem;
	int _multilineY;

private:
	Label* _sentencePieceLabel;
	Sprite* _sentencePieceSprite_bg;
	Sprite* _sentencePieceSprite_slot;
	Sprite* _sentencePieceSprite_slotEffect;

	bool _isEffectPlaying;
};


END_NS_SENTENCEBRIDGE;