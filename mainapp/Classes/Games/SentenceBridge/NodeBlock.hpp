//
//  NodeBlock.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/26/18.
//

#pragma once

#include "SentenceBridgeHeader.hpp"

BEGIN_NS_SENTENCEBRIDGE;

class NodeBlock : public Node
{
public:
	CREATE_FUNC(NodeBlock);
	virtual bool init() override;
	void removeObjects();

	void createBlock(string strLetter);
	void changeLetter(string strLetter);


	function<bool()> onTouchBegan;
	function<void(Touch* touch)> onTouchMoved;
	function<bool(Touch* touch)> onTouchEnded;
    function<bool(Touch* touch)> onTouchEnded_cancel;
	void blockMoveToWorldPosition(float duration, Vec2 worldPos);
	void blockResetPosition();

	Vec2 getBlockWorldPosition();
    
    void showAlphabet(bool bHasAlphabet);

private:
	//void setSoundFileName();
	void playLetterSound();

public:
	string _strLetter;
	//string _strSoundFileName;
    float _alpha;

	Sprite* _spriteShadhow;
	Sprite* _spriteBlock;
	Label* _labelLetter;
};


END_NS_SENTENCEBRIDGE;
