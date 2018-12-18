//
//  WordKicker_NodeBirdKicker.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//

#pragma once

#include "WordKickerHeader.hpp"

BEGIN_NS_WORDKICKER;

class WordKicker_NodeBirdKicker : public Node
{
public:
	CREATE_FUNC(WordKicker_NodeBirdKicker);
	virtual bool init() override;
	void removeObjects();

	void createSprites();
	
	void startNewProblem();
	
	void stopAnimation();
	void setAnimation_1_idle();
	void setAnimation_2_kick_success();
	void setAnimation_3_kick_fail();
	void setAnimation_debug_next();


private:
	


public:
	int _curAnimation;


private:
	Node* _node;
	Sprite* _spriteShadow;
	Sprite* _spriteBirdKicker;

	Action* _currentActionAnimation;
};


END_NS_WORDKICKER;