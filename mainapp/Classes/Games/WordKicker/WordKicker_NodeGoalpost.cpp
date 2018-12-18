//
//  WordKicker_NodeGoalpost.cpp
//  KitkitSchool
//
//  Created by MilliSoftGames on 8/25/18.
//


#include "WordKicker_NodeGoalpost.hpp"

#include "Utils/TodoUtil.h"


BEGIN_NS_WORDKICKER;

bool WordKicker_NodeGoalpost::init()
{
	if (!Node::init()) return false;
	return true;
}

void WordKicker_NodeGoalpost::removeObjects()
{
	_spriteGoalpost_b = nullptr;
	_spriteGoalpost_f = nullptr;

	removeAllChildren();
}

void WordKicker_NodeGoalpost::createSprites()
{
	_spriteGoalpost_b = Sprite::create(resourcePath + "goalpost_b.png");
	_spriteGoalpost_b->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteGoalpost_b->setPosition(627, -392);
	addChild(_spriteGoalpost_b);

	_spriteGoalpost_net = Sprite::create(resourcePath + "goalpost_net_1.png");
	_spriteGoalpost_net->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteGoalpost_net->setPosition(627, -392);
	addChild(_spriteGoalpost_net);
	_spriteGoalpost_net->setGlobalZOrder(1);

	_spriteGoalpost_f = Sprite::create(resourcePath + "goalpost.png");
	_spriteGoalpost_f->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_spriteGoalpost_f->setPosition(627, -392);
	addChild(_spriteGoalpost_f);
	_spriteGoalpost_f->setGlobalZOrder(2);



	Rect textureRect = _spriteGoalpost_net->getTextureRect();
	Vector<SpriteFrame*> animFrames;

	animFrames.clear();
	animFrames.pushBack(SpriteFrame::create(resourcePath + "goalpost_net_1.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "goalpost_net_2.png", textureRect));
	animFrames.pushBack(SpriteFrame::create(resourcePath + "goalpost_net_3.png", textureRect));
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	animation->setLoops(-1);

	_spriteGoalpost_net->runAction(Animate::create(animation));

	stopGoalAnimation();
}

void WordKicker_NodeGoalpost::startNewProblem()
{
	stopGoalAnimation();
}

void WordKicker_NodeGoalpost::startGoalAnimation()
{
	_spriteGoalpost_net->setVisible(true);
}

void WordKicker_NodeGoalpost::stopGoalAnimation()
{
	_spriteGoalpost_net->setVisible(false);
}

void WordKicker_NodeGoalpost::resetGlobalZOrder()
{
	_spriteGoalpost_net->setGlobalZOrder(0);
	_spriteGoalpost_f->setGlobalZOrder(0);
}

END_NS_WORDKICKER;