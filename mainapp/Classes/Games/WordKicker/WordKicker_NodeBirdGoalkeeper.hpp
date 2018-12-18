//
//  WordKicker_NodeBirdGoalkeeper.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//

#pragma once

#include "WordKickerHeader.hpp"

BEGIN_NS_WORDKICKER;

class WordKicker_NodeBirdGoalkeeper : public Node
{
public:
	CREATE_FUNC(WordKicker_NodeBirdGoalkeeper);
	virtual bool init() override;
	void removeObjects();

	void createSprites();
	
	void startNewProblem();

	void stopAnimation();
	void setAnimation_5_idle();
	void setAnimation_6_speak();
	void setAnimation_7_1_goal();
	void setAnimation_7_2_goal();
	void setAnimation_feather();
	void setAnimation_debug_next();

	void doJump();
	void doFly();

	void setEyesTarget(Vec2 vTargetWorldPos);
	void setEyesTarget_onlyX(Vec2 vTargetWorldPos);
private:


public:
	int _curAnimation;

private:
	Node* _node;
	Sprite* _spriteShadow;
	Sprite* _spriteFeather;
	Sprite* _spriteBirdGoalkeeper;
	Sprite* _spriteEyes;

	Action* _currentActionAnimation;
};


END_NS_WORDKICKER;