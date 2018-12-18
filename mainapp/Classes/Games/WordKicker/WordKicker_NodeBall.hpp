//
//  WordKicker_NodeBall.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//

#pragma once

#include "WordKickerHeader.hpp"

BEGIN_NS_WORDKICKER;

class WordKicker_NodeBall : public Node
{
public:
	CREATE_FUNC(WordKicker_NodeBall);
	virtual bool init() override;
	void removeObjects();

	void createSprites();
	
	void startNewProblem();
	void startNewProblem_notmove();

	Vec2 GetBallPosition_world();
	Vec2 GetBallPosition_local();

	bool processBall_wronganswer(float fTime, float fDeltaTime);
	bool processBall_timeover(float fTime, float fDeltaTime);
	bool processBall_correctanswer_1(float fTime, float fDeltaTime);
	bool processBall_correctanswer_2(float fTime, float fDeltaTime);
	bool processBall_correctanswer_final(float fTime, float fDeltaTime);

private:
	void showEffect(int nEffect);
	void stopEffect();

	float getRatio(float fTime, float fFrom, float fTo);
	


	


private:
	Node* _node;
	Sprite* _spriteShadow;
	Node* _nodeBall;
	vector<Sprite*> _spriteEffectList;
	Sprite* _spriteBall;

	float _ballBaseY;

};


END_NS_WORDKICKER;