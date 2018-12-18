//
//  NodeCar.hpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/29/18.
//

#pragma once

#include "SentenceBridgeHeader.hpp"

BEGIN_NS_SENTENCEBRIDGE;

class NodeCar : public Node
{
public:
	CREATE_FUNC(NodeCar);
	virtual bool init() override;
	void removeObjects();

	void createCar(float posX);
	void carMoveTo(float fDuration, float posX, bool bIsLast);

private:
	void makeSweat();
	void clearSweat();
	void makeHeart();
	void clearHeart();


private:
	Sprite* _spriteCar;
	Size _carSize;
	vector<Sprite*> _spriteSweatList;
	vector<Sprite*> _spriteHeartList;
};


END_NS_SENTENCEBRIDGE;
