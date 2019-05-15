//
//  NodeCar.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 7/29/18.
//


#include "NodeCar.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_SENTENCEBRIDGE;

bool NodeCar::init()
{
	if (!Node::init()) return false;
	return true;
}

void NodeCar::removeObjects()
{
	_spriteSweatList.clear();
	_spriteHeartList.clear();
	_spriteCar = nullptr;

	removeAllChildren();
}

void NodeCar::createCar(float posX)
{
	string strCarSpriteName = StringUtils::format("car-%02d.png", random<int>(1, 7));
	_spriteCar = Sprite::create(resourcePath + strCarSpriteName);
	_spriteCar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteCar->setPosition(posX, -20);
	addChild(_spriteCar);
	_carSize = _spriteCar->getContentSize();

	_spriteCar->runAction(
		RepeatForever::create(
			Sequence::create(
				JumpBy::create(0.1, Vec2::ZERO, 5, 1),
				DelayTime::create(0.3),
				nullptr
			)
		)
	);


	makeSweat();
}

void NodeCar::makeSweat()
{
	Vec2 pos1 = _spriteCar->getPosition();
	pos1.x += _carSize.width / 2;
	pos1.y += _carSize.height - 90;
	Sprite* spriteSweat1 = Sprite::create(resourcePath + "car-sweat-01.png");
	spriteSweat1->setPosition(pos1);
	addChild(spriteSweat1);

	Vec2 pos2 = _spriteCar->getPosition();
	pos2.x += _carSize.width / 2 - 30;
	pos2.y += _carSize.height - 50;
	Sprite* spriteSweat2 = Sprite::create(resourcePath + "car-sweat-02.png");
	spriteSweat2->setPosition(pos2);
	addChild(spriteSweat2);

	_spriteSweatList.push_back(spriteSweat1);
	_spriteSweatList.push_back(spriteSweat2);



	ccBezierConfig bc;
	bc.controlPoint_1 = Vec2(0, 30);
	bc.controlPoint_2 = Vec2(80, 30);
	bc.endPosition = Vec2(100, -20);

	spriteSweat1->setOpacity(0);
	spriteSweat1->runAction(
		RepeatForever::create(
			Sequence::create(
				//BezierBy::create(1, bc),
				EaseOut::create(MoveBy::create(1, Vec2(28,0)),3),
				DelayTime::create(1),
				Place::create(pos1),
				nullptr
			)
		)
	);
	spriteSweat1->runAction(
		RepeatForever::create(
			Sequence::create(
				FadeTo::create(0.1, 255),
				DelayTime::create(0.7),
				FadeOut::create(0.2),
				DelayTime::create(1),
				nullptr
			)
		)
	);



	bc.controlPoint_1 = Vec2(0, 50);
	bc.controlPoint_2 = Vec2(80, 50);
	bc.endPosition = Vec2(100, 20);

	spriteSweat2->setOpacity(0);
	spriteSweat2->runAction(
		RepeatForever::create(
			Sequence::create(
				DelayTime::create(0.3),
				//BezierBy::create(1, bc),
				EaseOut::create(MoveBy::create(1, Vec2(20, 20)),3),
				DelayTime::create(0.7),
				Place::create(pos2),
				nullptr
			)
		)
	);
	spriteSweat2->runAction(
		RepeatForever::create(
			Sequence::create(
				DelayTime::create(0.3),
				FadeTo::create(0.1, 255),
				DelayTime::create(0.7),
				FadeOut::create(0.2),
				DelayTime::create(0.7),
				nullptr
			)
		)
	);
}

void NodeCar::clearSweat()
{
	for (auto spriteSweat : _spriteSweatList)
	{
		removeChild(spriteSweat, true);
	}
	_spriteSweatList.clear();
}

void NodeCar::makeHeart()
{
	Vec2 pos = _spriteCar->getPosition();
	pos.x += random(-20.0f, 20.0f);
	pos.y += _carSize.height + random(-20.0f, 20.0f);

	string strHeartSpriteName = StringUtils::format("car-heart-%02d.png", random<int>(1, 3));
	Sprite* spriteHeart = Sprite::create(resourcePath + strHeartSpriteName);
	spriteHeart->setPosition(pos);
	addChild(spriteHeart);

	_spriteHeartList.push_back(spriteHeart);


	spriteHeart->runAction(
		RepeatForever::create(
			Sequence::create(
				JumpBy::create(1, Vec2(random(-100.0f, -20.0f), random(30.0f, 80.0f)), random(30.0f, 80.0f), 1),
				nullptr
			)
		)
	);
	spriteHeart->runAction(
		RepeatForever::create(
			Sequence::create(
				DelayTime::create(0.5),
				FadeOut::create(0.5),
				nullptr
			)
		)
	);
}

void NodeCar::clearHeart()
{
	for (auto spriteHeart : _spriteHeartList)
	{
		removeChild(spriteHeart, true);
	}
	_spriteSweatList.clear();
}

void NodeCar::carMoveTo(float fDuration, float posX, bool bIsLast)
{
	clearSweat();

	Vec2 moveBy = Vec2(posX - _spriteCar->getPosition().x, 0);
	_spriteCar->runAction(
		Sequence::create(
			EaseInOut::create(MoveBy::create(fDuration, moveBy), 2),
			CallFunc::create([this]() {clearHeart(); makeSweat(); }),
			nullptr
		)
	);

	for (float fTime = 0.5f; fTime < fDuration; fTime += random(0.08f, 0.2f))
	{
		runAction(
			Sequence::create(
				DelayTime::create(fTime),
				CallFunc::create([this]() {makeHeart(); }),
				nullptr
			)
		);
	}
    
    if (bIsLast)
    {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/cardrive_gone.m4a");
    }
    else
    {
        GameSoundManager::getInstance()->playEffectSound("Common/Sounds/Effect/cardrive.m4a");
    }
}

END_NS_SENTENCEBRIDGE;
