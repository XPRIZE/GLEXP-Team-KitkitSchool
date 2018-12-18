//
//  WordKicker_NodeGoalpost.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//

#pragma once

#include "WordKickerHeader.hpp"

BEGIN_NS_WORDKICKER;

class WordKicker_NodeGoalpost : public Node
{
public:
	CREATE_FUNC(WordKicker_NodeGoalpost);
	virtual bool init() override;
	void removeObjects();

	void createSprites();
	
	void startNewProblem();

	void startGoalAnimation();
	void stopGoalAnimation();
	void resetGlobalZOrder();

private:



private:
	Sprite* _spriteGoalpost_b;
	Sprite* _spriteGoalpost_net;
	Sprite* _spriteGoalpost_f;
};


END_NS_WORDKICKER;